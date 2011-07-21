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
 * Filename:
 * ---------
 * wgui_inline_edit.C
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Functions for Inline items.
 * Author:
 * -------
 * -------
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**********************************************************************************
   Filename:      wgui_inline_edit.c
   Date Created:  June-17-2003
   Contains:      Implementation for inline editing
**********************************************************************************/

#include "globaldefs.h"
#include "custdatares.h"
#include "custdataprots.h"
#include "gui_themes.h"
#include "gui_config.h"
#include "wgui_categories.h"
#include "wgui_status_icons.h"
#include "wgui_datetime.h"
#include "mmi_features.h"
#include "timerevents.h"
#include "debuginitdef.h"
 
#include "historygprot.h"
#include "wgui_inline_edit.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_defs.h"
#include "commonscreens.h"
#include "gui_setting.h"
 
#include "wap.h"
/*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-3-10 14:3 */
#if 0
#include "pocgprot.h"
#endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-3-10 14:3 */
#include "messagesresourcedata.h"
#include "satdefs.h"
#include "wgui_draw_manager.h"
#include "wgui_virtual_keyboard.h"
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
#include "editorpen.h"
static BOOL mmi_pen_editor_is_closecategory57;
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 
 
extern U16 StopMyTimer(U16 nTimerId);
extern U16 StartMyTimerInt(U16 nTimerId, U32 nTimeDuration, oslTimerFuncPtr TimerExpiry, U8 alignment);

/* CSD end */

 
extern BOOL r2lMMIFlag;

/* CSD end */

 
extern GDI_HANDLE MOMT_animation_handle;

 

 
extern S16 MMI_current_input_ext_type;
extern U8 MMI_allow_only_english_input_modes;

 
/*
 * #ifdef __MMI_T9__
 * extern void (*t9_inputbox_LSK_function)(void);
 * extern void InuptMethodEnterCategory5(void);
 *  
 *  
 * //CSD end
 * #elif defined __MMI_ZI__
 * extern void (*zi_inputbox_LSK_function)(void);
 * extern void ZiInuptMethodEnterCategory5(void);
 * #elif defined __MMI_KA__
 * extern void (*ka_inputbox_LSK_function)(void);
 * extern void KonkaInuptMethodEnterCategory5(void);
 * #elif defined __MMI_ITAP__
 * extern void (*itap_inputbox_LSK_function)(void);
 * extern void ItapInuptMethodEnterCategory5(void);
 * #endif
 */
/* CSD end */
#ifdef __MMI_T9__
#include "t9definition.h"
#include "t9api.h"
extern void T9SetInputboxLSKFunction(FuncPtr f);
extern void InuptMethodEnterCategory5(void);
extern void InuptMethodExitCategory5(void);
extern void T9InputMethodExit(void);
#elif defined __MMI_ZI__
extern void ZiSetInputboxLSKFunction(FuncPtr f);
extern void ZiInuptMethodEnterSinglelineInputBox(void);
extern void ZiInuptMethodExitSinglelineInputBox(void);
extern void ZiInuptMethodEnterCategory5(void);
extern void ZiInuptMethodExitCategory5(void);
extern void ZiInputMethodExit(void);
#elif defined __MMI_KA__
extern void KonKaSetInputboxLSKFunction(FuncPtr f);
extern void KonkaInuptMethodEnterSinglelineInputBox(void);
extern void KonkaInuptMethodExitSinglelineInputBox(void);
extern void KonkaInuptMethodEnterCategory5(void);
extern void KonkaInuptMethodExitCategory5(void);
extern void KonkaInputMethodExit(void);
#elif defined __MMI_ITAP__
extern void ItapSetInputboxLSKFunction(FuncPtr f);

#include "itaphmidef.h"
#include "itaphmistruct.h"
#include "itaphmigprot.h"
#endif 

 
#ifdef __MMI_MULTITAP_KEY_0__
#define ENABLE_DIALER_ZERO_KEY_MULTITAP            1    /* 0 */
#else 
#define ENABLE_DIALER_ZERO_KEY_MULTITAP            0
#endif 
/* CSD end */

/* Amount of time (milliseconds) to wait before displaying the
   current input method pop up for inline text edit items               */
#define INLINE_INPUT_METHOD_DISPLAY_WAIT        (1000)

/* Amount of time (milliseconds) to display the current input method    */
#define INLINE_INPUT_METHOD_HIDE_WAIT           (2000)

/*----------------------------------------------------------------------------
   Inline edit data
----------------------------------------------------------------------------*/

 
// #define ENABLE_INLINE_SINGLELINE_INPUTBOX_FRAME_SKIPPING  1
 

/* History IDs for inline items  */

#define INLINE_TEXT_EDIT_HISTORY_ID                1
#define INLINE_SELECT_HISTORY_ID                   3
#define INLINE_USER_DEFINED_SELECT_HISTORY_ID      4
#define INLINE_DATE_HISTORY_ID                     5
#define INLINE_TIME_HISTORY_ID                     6
#define INLINE_DOW_SELECT_HISTORY_ID               7
#define INLINE_TIME_PERIOD_HISTORY_ID              8
#define INLINE_IP4_HISTORY_ID                      9
#define INLINE_FULL_SCREEN_EDIT_HISTORY_ID         10
#define INLINE_MULTI_LINE_EDIT_HISTORY_ID          11
#define INLINE_COLOR_SELECT_SCREEN_ID              12

wgui_inline_item wgui_inline_items[MAX_INLINE_ITEMS];
wgui_inline_item *current_wgui_inline_item = NULL;

 
extern pBOOL flag_use_current_input_type;

 
 
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
extern void gui_disable_inline_bottom_line_draw(void);
extern void gui_enable_inline_bottom_line_draw(void);

extern void gui_enable_fixed_list_line_draw();
extern void gui_disable_fixed_list_line_draw(void);
extern BOOL line_draw;
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
 
S32 wgui_n_inline_items = 0;
S32 wgui_inline_item_highlighted_index = -1;
S32 wgui_inline_item_key_code = -1;
S32 wgui_inline_item_key_event = -1;
S32 wgui_inline_item_x = 0;
S32 wgui_inline_item_y = 0;
S32 wgui_inline_item_width = 0;
S32 wgui_inline_item_height = 0;
U8 wgui_inline_item_handled = 0;
S32 wgui_current_inline_item_type = 0;
U8 *wgui_current_history_buffer = NULL;
U8 wgui_disable_inline_input_method_hints = 1;

#if defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__)
S8 MMI_multi_line_inputbox_present = 0;
#endif 

/* Added to handle (Back/Done) on RSK  */
U8 wgui_inline_list_menu_changed = 0;
U8 wgui_inline_list_menu_disable_done = 0;
void (*wgui_inline_edit_BACK_function) (void) = NULL;
void (*wgui_inline_edit_DONE_function) (void) = NULL;
void (*wgui_inline_edit_LSK_function) (void) = NULL;

extern UI_single_line_input_box_theme date_time_input_theme;
extern UI_filled_area date_time_input_background_filler;
extern UI_filled_area time_period_input_background_filler;

extern void set_current_input_box_type(mmi_editor_inoutbox_enum input_box_type);

#define MAX_INLINE_TEXT_EDIT_ITEMS                    15
#define MAX_INLINE_TEXT_EDIT_BUFFER_SIZE              124       /* For Email */
#define MAX_INLINE_FULL_SCREEN_TEXT_EDIT_BUFFER_SIZE     512

S32 n_inline_text_edit_items = 0;
S8 inline_multiline_complete = 0;
wgui_inline_item_text_edit wgui_inline_text_edit_items[MAX_INLINE_TEXT_EDIT_ITEMS];
U8 inline_text_edit_mask_buffers[MAX_INLINE_TEXT_EDIT_ITEMS][MAX_INLINE_TEXT_EDIT_BUFFER_SIZE];
UI_string_type inline_full_screen_edit_saved_title = (UI_string_type) NULL;
UI_string_type inline_full_screen_edit_saved_LSK_label = (UI_string_type) NULL;
UI_string_type inline_full_screen_edit_saved_RSK_label = (UI_string_type) NULL;
PU8 inline_full_screen_edit_saved_LSK_icon = (PU8) NULL;
PU8 inline_full_screen_edit_saved_RSK_icon = (PU8) NULL;
PU8 inline_full_screen_edit_saved_title_icon = (PU8) NULL;
U8 inline_full_screen_edit_changed = 0;
wgui_inline_item_text_edit *current_wgui_inline_text_edit_item;
U8 **current_wgui_inline_text_edit_text_p;
U8 wgui_inline_full_screen_text_edit_buffer[MAX_INLINE_FULL_SCREEN_TEXT_EDIT_BUFFER_SIZE];

void handle_inline_full_screen_edit_cancel(void);
void inline_edit_prepare_mask_buffer(U8 *, U8 *);
void wgui_inline_edit_handle_multiline_inputbox_input(void);
void handle_inline_full_screen_edit_complete(void);
void handle_inline_full_screen_edit_close(void);
void wgui_inline_edit_default_list_highlight_handler(S32 item_index);

/** New Image Text Control ***/
#if defined __MMI_PLUTO_GPRS__

#define MAX_INLINE_IMAGE_TEXT_ITEMS                   10
#define  IMAGE_TEXT_CONTROL_TEXT_X        26
#define  IMAGE_TEXT_CONTROL_TEXT_WIDTH    83

U8 inline_scroll_flag = FALSE;
S32 image_text_control_scroll_x = 0;
scrolling_text wgui_inline_scroll_text;
PU8 inline_last_image = NULL;
S32 inline_y_pos_image = 0;
U8 inline_image_text_edit_flag = 0;

void draw_image_text_control(
        wgui_inline_image_text *control,
        S32 x1,
        S32 y1,
        S32 iwidth,
        S32 iheight,
        PU8 image1,
        PU8 image2,
        PU8 image3,
        U8 *text,
        U8 highlight_flag);
void wgui_show_image_text_item(S32 x1, S32 y1, S32 iwidth, S32 iheight, wgui_inline_item *item);
wgui_inline_image_text wgui_inline_image_text_items[MAX_INLINE_IMAGE_TEXT_ITEMS];
wgui_inline_image_text *current_image_text_control = NULL;
S32 n_inline_image_text = 0;
void handle_inline_full_screen_edit(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer);
void handle_inline_full_screen_imagetext_edit(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer);

#endif /* defined __MMI_PLUTO_GPRS__ */ 

#ifdef __MMI_TOUCH_SCREEN__

typedef enum
{
    /* Ignore pen events */
    WGUI_INLINE_PEN_HANDLER_NONE,
    /* Prepare to change highlight on Pen Up. Do nothing else. */
    WGUI_INLINE_PEN_HANDLER_SWITCH_ITEM,
    /* Redirect pen events to scrollbar */
    WGUI_INLINE_PEN_HANDLER_SCROLLBAR,
    /* Redirect pen events to the current menu item */
    WGUI_INLINE_PEN_HANDLER_CURRENT_ITEM,
    /* Wait long press animation to invoke LSK */
    WGUI_INLINE_PEN_HANDLER_WAIT_LONGPRESS,
    /* Redirect pen events to fullscreen editor */
    WGUI_INLINE_PEN_HANDLER_FULLSCREEN_EDITOR
} wgui_inline_pen_handler_type_enum;

static wgui_inline_pen_handler_type_enum wgui_inline_pen_handler_type;
static S32 wgui_inline_pen_down_item_index;

static BOOL(*wgui_current_inline_item_pen_function) (mmi_pen_event_type_enum pen_event, S16 x, S16 y,
                                                     gui_inline_item_pen_enum *item_event) =
    UI_dummy_inline_item_pen_function;

#endif /* __MMI_TOUCH_SCREEN__ */ 

 
#if defined (__MMI_WALLPAPER_ON_BOTTOM__)
static UI_image_ID_type  inline_edit_scr_bg_ID;
static S8 *inline_edit_scr_bg_filename;
static S32 inline_edit_scr_bg_x, inline_edit_scr_bg_y;
static U8 inline_edit_scr_bg_opacity;
#endif /* defined (__MMI_WALLPAPER_ON_BOTTOM__) */ 
 

extern icontext_button MMI_softkeys[];

 
static void (*inline_item_changed_handler) (U16 index);

 

/************************/
void redraw_inline_full_screen_edit(void);

void inline_edit_phone_number_input_complete_multitap_star_key(void);

 
void inline_edit_phone_number_input_complete_multitap_zero_key(void);

 

void cat57_virtual_keypad_callback(void);

/* Used to highlight the same item again after editing (ex: After return from full screen edit  */
void (*inline_edit_current_highlight_handler) (S32 item_index) = MMI_dummy_highlight_handler;

UI_filled_area temp_inline_list_menu_background_filler = 
{
    UI_FILLED_AREA_TYPE_COLOR,
    UI_NULL_IMAGE,
    NULL,
     
#ifdef __MMI_DEFAULT_THEME_3__
    {206, 206, 206, 100},
#else 
    {240, 240, 240, 100},
#endif 
     
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {0, 0, 0, 0},
    0
};

UI_filled_area temp_inline_list_menu_normal_item_filler = {UI_FILLED_AREA_TYPE_COLOR,
    UI_NULL_IMAGE,
    NULL,
     
#ifdef __MMI_DEFAULT_THEME_3__
    {206, 206, 206, 100},
#else 
    {240, 240, 240, 100},
#endif 
     
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    0
};

UI_filled_area temp_inline_list_menu_selected_item_filler = {UI_FILLED_AREA_TYPE_COLOR,
    UI_NULL_IMAGE,
    NULL,
     
#ifdef __MMI_DEFAULT_THEME_3__
    {255, 255, 255, 100},
#else 
    {167, 232, 93, 100},
#endif 
     
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    0
};

UI_filled_area temp_inline_list_menu_disabled_item_filler = {UI_FILLED_AREA_TYPE_COLOR,
    UI_NULL_IMAGE,
    NULL,
    {192, 192, 192, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    0
};

UI_fixed_list_menu_theme wgui_inline_fixed_list_menu_theme = { &temp_inline_list_menu_background_filler,
    &temp_inline_list_menu_background_filler,
    5,
    UI_LIST_MENU_LOOP
};

UI_fixed_icontext_menuitem_theme wgui_inline_fixed_icontext_menuitem_theme =
    { &temp_inline_list_menu_normal_item_filler,
    &temp_inline_list_menu_disabled_item_filler,
    &temp_inline_list_menu_selected_item_filler,
    &temp_inline_list_menu_selected_item_filler,
    {0, 0, 0, 100},
    {0, 0, 0, 100},
    {0, 0, 0, 100},
    {0, 0, 0, 100},
#ifdef __MMI_MAINLCD_220X176__
    &MMI_small_font,
#else
    &MMI_default_font,
#endif    
    UI_MENUITEM_CENTER_TEXT_Y | UI_MENUITEM_CENTER_ICON_Y
};

UI_filled_area temp_inline_select_menu_filler = {UI_FILLED_AREA_TYPE_COLOR,
    NULL,
    NULL,
    {71, 127, 255, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {0, 0, 0, 0},
    0
};

UI_horizontal_select_theme temp_inline_select_menu_theme = { &temp_inline_select_menu_filler,
    UI_HORIZONTAL_SELECT_LOOP,
};

UI_fixed_text_menuitem_theme wgui_inline_select_fixed_text_menuitem_theme = { &temp_inline_list_menu_normal_item_filler,
    &temp_inline_list_menu_disabled_item_filler,
    &temp_inline_list_menu_selected_item_filler,
    &temp_inline_list_menu_selected_item_filler,
    {0, 0, 0, 100},
    {0, 0, 0, 100},
    {0, 0, 0, 100},
    {0, 0, 0, 100},
    &MMI_default_font,
    UI_MENUITEM_CENTER_TEXT_Y
};

 
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
U8 wgui_enable_inline_item_default_text = 0;

UI_filled_area temp_inline_edit_normal_filler =
    {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER | UI_FILLED_AREA_ROUNDED_BORDER,
    UI_NULL_IMAGE,
    NULL,
     
    {255, 229, 172, 100},
     
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {0, 0, 0, 0},
    0
};
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
 

/*----------------------------------------------------------------------------
   Key handlers for the inline fixed list
----------------------------------------------------------------------------*/

#define CATEGORY57_NO_KEY_FLAG         0
#define CATEGORY57_UP_ARROW_KEY_FLAG   1
#define CATEGORY57_DOWN_ARROW_KEY_FLAG 2

U8 wgui_inline_edit_arrow_key_flag = CATEGORY57_NO_KEY_FLAG;
void (*clear_inline_item_keys) (void) = UI_dummy_function;
void (*redraw_current_inline_item) (void) = UI_dummy_function;
void (*complete_inline_item_edit) (void) = UI_dummy_function;
void register_inline_fixed_list_keys(void);
void ShowCommonCategoryInlineEdit(UI_string_type title, U16 title_icon, U16 left_softkey, U16 left_softkey_icon,
                                  U16 right_softkey, U16 right_softkey_icon, S32 number_of_items, U16 *list_of_icons,
                                  InlineItem *list_of_items, S32 highlighted_item, U8 *history_buffer,
                                  void (*highlight_handler) (S32 item_index),
                                  void (*unhighlight_handler) (S32 item_index), U16 history_id);

 
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
void inline_item_default_text_show_background(S32 x1, S32 y1, S32 x2, S32 y2);
void inline_text_edit_show_background(single_line_input_box *b);
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
 

/* For transparent color filler */
 
// This part of code should be removed
#if 0
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
#endif
#endif /* 0 */ 
 



/*****************************************************************************
 * FUNCTION
 *  get_wgui_inline_list_menu_status
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 get_wgui_inline_list_menu_status(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return wgui_inline_list_menu_changed;
}


/*****************************************************************************
 * FUNCTION
 *  set_wgui_inline_list_menu_status
 * DESCRIPTION
 *  
 * PARAMETERS
 *  status      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_wgui_inline_list_menu_status(S32 status)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_list_menu_changed = (U8) status;
}

/*** Image Text Control *****/
#if defined __MMI_PLUTO_GPRS__


/*****************************************************************************
 * FUNCTION
 *  ShowAsControl
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void ShowAsControl(InlineItem *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->show_img_flag = TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineFullScreenEditCustomFunctionImageText
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineFullScreenEditCustomFunctionImageText(InlineItem *item, void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_image_text *image_item = (wgui_inline_image_text*) item->item;
    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) image_item->inline_full_text;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i->custom_setup_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_edit_complete_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  buffer          [?]         
 *  buffer_size     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UI_dummy_edit_complete_handler(U8 *buffer, S32 buffer_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(buffer);
    UI_UNUSED_PARAMETER(buffer_size);
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  inline_image_text_redraw_image_by_pen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  i       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void inline_image_text_redraw_image_by_pen(wgui_inline_image_text *i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 index = i->pen_down_image_index;
    PU8 image = NULL;    
    S32 image_y_offset = 0;
    S32 x1, y1, x2, y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index < 0)
    {
        return;
    }

    gdi_layer_push_clip();
    gdi_layer_reset_clip();
    gdi_layer_lock_frame_buffer();

    x1 = i->image_left_top[index].x;
    y1 = i->image_left_top[index].y;
    x2 = i->image_right_bottom[index].x;
    y2 = i->image_right_bottom[index].y;

    gui_draw_filled_area(x1, y1, x2, y2 + 1, wgui_inline_select_fixed_text_menuitem_theme.selected_filler);

    if (index == i->pen_curr_pressed_image_index)
    {
        image_y_offset++;
    }

    if (index == 0)
    {
        image = i->image2;
    }
    else if (index == 1)
    {
        image = i->image3;
    }
    else
    {
        MMI_ASSERT(0);
    }

    gui_show_transparent_image(x1, y1 + image_y_offset, image, 0);

    gui_pop_clip();

    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(x1, y1, x2, y2 + 1);
}


/*****************************************************************************
 * FUNCTION
 *  inline_image_text_pen_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pen_event       [IN]        
 *  x               [IN]        
 *  y               [IN]        
 *  item_event      [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL inline_image_text_pen_handler(
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_inline_item_pen_enum *item_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret = MMI_TRUE;
    wgui_inline_image_text *i = current_image_text_control;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *item_event = GUI_INLINE_ITEM_PEN_NONE;

    if (!i)
    {
        MMI_DBG_ASSERT(0);
        return MMI_FALSE;
    }

    switch (pen_event)
    {
        case MMI_PEN_EVENT_DOWN:
            i->pen_down_image_index = -1;
            if (i->image2 && i->image_callback[0] && x >= i->image_left_top[0].x && y >= i->image_left_top[0].y &&
                x <= i->image_right_bottom[0].x && y <= i->image_right_bottom[0].y)
            {
                i->pen_down_image_index = 0;
                i->pen_curr_pressed_image_index = 0;
                inline_image_text_redraw_image_by_pen(i);
            }
            else if (i->image3 && i->image_callback[1] && x >= i->image_left_top[1].x && y >= i->image_left_top[1].y &&
                     x <= i->image_right_bottom[1].x && y <= i->image_right_bottom[1].y)
            {
                i->pen_down_image_index = 1;
                i->pen_curr_pressed_image_index = 1;
                inline_image_text_redraw_image_by_pen(i);
            }
            else
            {
                ret = MMI_FALSE;
            }
            break;

        case MMI_PEN_EVENT_MOVE:
            if (i->pen_down_image_index >= 0)
            {
                S32 index = i->pen_down_image_index;

                if (i->pen_curr_pressed_image_index == i->pen_down_image_index)
                {
                    if (!(x >= i->image_left_top[index].x && y >= i->image_left_top[index].y &&
                          x <= i->image_right_bottom[index].x && y <= i->image_right_bottom[index].y))
                    {
                        i->pen_curr_pressed_image_index = -1;
                        inline_image_text_redraw_image_by_pen(i);
                    }
                }
                else
                {
                    if (x >= i->image_left_top[index].x && y >= i->image_left_top[index].y &&
                        x <= i->image_right_bottom[index].x && y <= i->image_right_bottom[index].y)
                    {
                        i->pen_curr_pressed_image_index = index;
                        inline_image_text_redraw_image_by_pen(i);
                    }
                }
            }
            break;

        case MMI_PEN_EVENT_UP:
            if (i->pen_down_image_index >= 0 && i->pen_curr_pressed_image_index >= 0)
            {
                i->pen_curr_pressed_image_index = -1;
                inline_image_text_redraw_image_by_pen(i);

                if (i->image_callback[i->pen_down_image_index])
                {
                    (i->image_callback[i->pen_down_image_index]) ();
                }
            }
            break;

        case MMI_PEN_EVENT_ABORT:
            if (i->pen_down_image_index >= 0 && i->pen_curr_pressed_image_index >= 0)
            {
                i->pen_curr_pressed_image_index = -1;
                inline_image_text_redraw_image_by_pen(i);
            }
            break;
        default:
            break;
    }

    return ret;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemImageText
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item            [?]         
 *  text            [?]         
 *  image1          [IN]        
 *  image2          [IN]        
 *  image3          [IN]        
 *  buf_size        [IN]        
 *  title           [IN]        
 *  title_icon      [IN]        
 *  input_type      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemImageText(
        InlineItem *item,
        U8 *text,
        PU8 image1,
        PU8 image2,
        PU8 image3,
        S32 buf_size,
        U16 title,
        U16 title_icon,
        S16 input_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_image_text *image_item;
    wgui_inline_item_text_edit *i;
    UI_buffer_type mask_buffer;
    S16 t = input_type & INPUT_TYPE_MASK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_inline_image_text >= MAX_INLINE_IMAGE_TEXT_ITEMS)
    {
        return;
    }
    if (n_inline_text_edit_items >= MAX_INLINE_TEXT_EDIT_ITEMS)
    {
        return;
    }

    item->item = (void*)&wgui_inline_image_text_items[n_inline_image_text];
    item->flags = INLINE_ITEM_IMAGE_TEXT_TYPE;
    item->show_img_flag = FALSE;
    n_inline_image_text++;
    image_item = (wgui_inline_image_text*) item->item;
    image_item->flags = 0;
    image_item->buffer_size = buf_size;
    image_item->image1 = image1;
    image_item->image2 = image2;
    image_item->image3 = image3;
    image_item->text = text;
    item->handler = handle_inline_full_screen_imagetext_edit;   /* wgui_show_image_text_item;//handle_inline_full_screen_imagetext_edit ; */
#ifdef __MMI_TOUCH_SCREEN__
    memset(image_item->image_callback, 0, sizeof(image_item->image_callback));
    memset(image_item->image_left_top, 0, sizeof(image_item->image_left_top));
    memset(image_item->image_right_bottom, 0, sizeof(image_item->image_right_bottom));
    image_item->pen_down_image_index = -1;
    image_item->pen_curr_pressed_image_index = -1;
#endif /* __MMI_TOUCH_SCREEN__ */ 

    image_item->inline_full_text = (void*)&wgui_inline_text_edit_items[n_inline_text_edit_items];
    mask_buffer = inline_text_edit_mask_buffers[n_inline_text_edit_items];
    n_inline_text_edit_items++;
    i = (wgui_inline_item_text_edit*) image_item->inline_full_text;
    i->title = title;
    i->title_icon = title_icon;
    i->buffer = text;
    i->buffer_size = buf_size;
    i->flags = 0;
    i->input_type = input_type;
    item->text_p = i->buffer;
    i->edit_complete_callback = UI_dummy_edit_complete_handler;
    i->custom_setup_function = NULL;
    i->validation_callback = UI_dummy_validation_function;
    if ((t == INPUT_TYPE_NUMERIC_PASSWORD) || (t == INPUT_TYPE_ALPHANUMERIC_PASSWORD) ||
        (t == INPUT_TYPE_SAT_NUMERIC_PASSWORD))
    {
        inline_edit_prepare_mask_buffer(text, mask_buffer);
        i->mask_buffer = mask_buffer;
        item->text_p = mask_buffer;
    }
    /* UCS2_count==-1 means this field hasn't been edited */
    i->UCS2_count = -1;

}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  SetInlineImageTextPenHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                [?]         
 *  image2_callback     [IN]        
 *  image3_callback     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineImageTextPenHandler(InlineItem *item, FuncPtr image2_callback, FuncPtr image3_callback)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_image_text *image_item = (wgui_inline_image_text*) item->item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    image_item->image_callback[0] = image2_callback;
    image_item->image_callback[1] = image3_callback;
}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  wgui_inlinescrolling_timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inlinescrolling_timer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_handle_scrolling_text(&wgui_inline_scroll_text);
    if (inline_last_image)
    {
        gui_show_transparent_image(103, inline_y_pos_image, inline_last_image, 0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_scrolling_text_draw_background
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_scrolling_text_draw_background(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_draw_filled_area(x1, y1, x2, y2, (wgui_inline_select_fixed_text_menuitem_theme.selected_filler));
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_image_text_control
 * DESCRIPTION
 *  
 * PARAMETERS
 *  i       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_image_text_control(wgui_inline_image_text *i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    //S32 y1 = i->y ;
    //S32 height = 0;
    wgui_inline_item_text_edit *i_edit = (wgui_inline_item_text_edit*) i->inline_full_text;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    draw_image_text_control(i, i->x, i->y, i->width, i->height, i->image1, i->image2, i->image3, i_edit->buffer, 1);

}


/*****************************************************************************
 * FUNCTION
 *  redraw_image_text_control
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_image_text_control(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    wgui_show_image_text_control(current_image_text_control);
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(
        current_image_text_control->x,
        current_image_text_control->y,
        current_image_text_control->x + current_image_text_control->width - 1,
        current_image_text_control->y + current_image_text_control->height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  draw_image_text_control
 * DESCRIPTION
 *  
 * PARAMETERS
 *  control             [?]         
 *  x1                  [IN]        
 *  y1                  [IN]        
 *  iwidth              [IN]        
 *  iheight             [IN]        
 *  image1              [IN]        
 *  image2              [IN]        
 *  image3              [IN]        
 *  text                [?]         
 *  highlight_flag      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void draw_image_text_control(
        wgui_inline_image_text *control,
        S32 x1,
        S32 y1,
        S32 iwidth,
        S32 iheight,
        PU8 image1,
        PU8 image2,
        PU8 image3,
        U8 *text,
        U8 highlight_flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 width = 0, height = 0, char_height = 0, img_y1 = 0, text_y;
    color_t text_colr = gui_color(0, 0, 0);
    S32 temp_x = x1 + 1;
    S32 text_x = 0, text_width = 0, text_height = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_reset_clip();
    gui_reset_text_clip();

     
    if (!r2lMMIFlag)
    {
        if (image1)
        {
            gui_measure_image((PU8) image1, &width, &height);
            img_y1 = (MMI_INLINE_EDIT_MENUITEM_HEIGHT >> 1) - (height >> 1);
            gui_show_transparent_image(temp_x, (y1 + img_y1), image1, 0);
            temp_x = x1 + width + 4;
        }
        else
             
    #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        if (!IsInlineItemDefaultTextEnable())
    #endif 
             
            temp_x = x1 + MMI_fixed_icontext_menuitem.text_x;
        if (image2)
        {
            gui_measure_image(image2, &width, &height);
            img_y1 = (MMI_INLINE_EDIT_MENUITEM_HEIGHT >> 1) - (height >> 1);
            gui_show_transparent_image(temp_x, (y1 + img_y1), image2, 0);
            text_x = temp_x + width + 2;
            text_width = iwidth - text_x;
        #ifdef __MMI_TOUCH_SCREEN__
            if (control)
            {
                control->image_left_top[0].x = temp_x;
                control->image_left_top[0].y = y1 + img_y1;
                control->image_right_bottom[0].x = temp_x + width - 1;
                control->image_right_bottom[0].y = y1 + img_y1 + height - 1;
            }
        #endif /* __MMI_TOUCH_SCREEN__ */ 
        }
        else
        {
            text_x = temp_x;
            text_width = iwidth - text_x - x1 - 4;
        }
        if (image3)
        {
            gui_measure_image(image3, &width, &height);
            temp_x = x1 + 1 + iwidth - width - 2;
            img_y1 = (MMI_INLINE_EDIT_MENUITEM_HEIGHT >> 1) - (height >> 1);
            gui_show_transparent_image(temp_x, (y1 + img_y1), image3, 0);
            text_width = temp_x - text_x - 2;
        #ifdef __MMI_TOUCH_SCREEN__
            if (control)
            {
                control->image_left_top[1].x = temp_x;
                control->image_left_top[1].y = y1 + img_y1;
                control->image_right_bottom[1].x = temp_x + width - 1;
                control->image_right_bottom[1].y = y1 + img_y1 + height - 1;
            }
        #endif /* __MMI_TOUCH_SCREEN__ */ 
        }

    }
    else
    {
        temp_x = UI_device_width - 1;
        if (image1)
        {
            gui_measure_image((PU8) image1, &width, &height);
            img_y1 = (MMI_INLINE_EDIT_MENUITEM_HEIGHT >> 1) - (height >> 1);
            temp_x -= width;
            gui_show_transparent_image(temp_x, (y1 + img_y1), image1, 0);

        }
        else
        {
            temp_x = UI_device_width - MMI_fixed_icontext_menuitem.text_x;
        }

        if (image3)
        {
            gui_measure_image(image3, &width, &height);
            img_y1 = (MMI_INLINE_EDIT_MENUITEM_HEIGHT >> 1) - (height >> 1);
            temp_x -= (width + 2);
            gui_show_transparent_image(temp_x, (y1 + img_y1), image3, 0);
            text_x = temp_x - 2;
            text_width = iwidth - (UI_device_width - text_x);
        #ifdef __MMI_TOUCH_SCREEN__
            if (control)
            {
                control->image_left_top[1].x = temp_x;
                control->image_left_top[1].y = y1 + img_y1;
                control->image_right_bottom[1].x = temp_x + width - 1;
                control->image_right_bottom[1].y = y1 + img_y1 + height - 1;
            }
        #endif /* __MMI_TOUCH_SCREEN__ */ 
        }
        else
        {
            text_x = temp_x;
            text_width = iwidth - (UI_device_width - text_x) - 2;
        }

        if (image2)
        {
            gui_measure_image(image2, &width, &height);
            img_y1 = (MMI_INLINE_EDIT_MENUITEM_HEIGHT >> 1) - (height >> 1);
            temp_x = (UI_device_width - 1) - iwidth + 2;
            gui_show_transparent_image(temp_x, (y1 + img_y1), image2, 0);
            text_width = text_x - temp_x - 2 - width;
        #ifdef __MMI_TOUCH_SCREEN__
            if (control)
            {
                control->image_left_top[0].x = temp_x;
                control->image_left_top[0].y = y1 + img_y1;
                control->image_right_bottom[0].x = temp_x + width - 1;
                control->image_right_bottom[0].y = y1 + img_y1 + height - 1;
            }
        #endif /* __MMI_TOUCH_SCREEN__ */ 
        }

    }

    char_height = gui_get_character_height();

    gui_measure_string((UI_string_type) text, &width, &height);
    text_height = height;
    text_y = y1 + (MMI_INLINE_EDIT_MENUITEM_HEIGHT >> 1) - (height >> 1);
    if (highlight_flag)
    {
        text_colr = MMI_fixed_icontext_menuitem.focussed_text_color;
        if (width > text_width)
        {
            if (r2lMMIFlag)
            {
                gui_create_scrolling_text(&wgui_inline_scroll_text, text_x - text_width, text_y, text_width, text_height, (UI_string_type) text, wgui_inlinescrolling_timer, wgui_scrolling_text_draw_background, text_colr, text_colr);         
            }
            else
            {
                gui_create_scrolling_text(&wgui_inline_scroll_text, text_x, text_y, text_width, text_height, (UI_string_type) text, wgui_inlinescrolling_timer, wgui_scrolling_text_draw_background, text_colr, text_colr);      
            }
            gui_show_scrolling_text(&wgui_inline_scroll_text);
        }
        else
        {
            if (r2lMMIFlag)
            {
                gdi_layer_set_clip(text_x - text_width, text_y - 1, text_x, text_y + text_height + 1);
            }
            else
            {
                gdi_layer_set_clip(text_x, text_y - 1, text_x + text_width, text_y + text_height + 1);
            }
            gui_move_text_cursor(text_x, text_y);
            gui_set_text_color(text_colr);
            gui_print_text((UI_string_type) text);
        }
    }
    else
    {
        text_colr = MMI_fixed_icontext_menuitem.normal_text_color;
        if (r2lMMIFlag)
        {
            gdi_layer_set_clip(text_x - text_width, text_y - 1, text_x, text_y + text_height + 1);
        }
        else
        {
            gdi_layer_set_clip(text_x, text_y - 1, text_x + text_width, text_y + text_height + 1);
        }
        gui_move_text_cursor(text_x, text_y);
        gui_set_text_color(text_colr);
        gui_print_text((UI_string_type) text);
    }
    gui_pop_clip();
    UI_UNUSED_PARAMETER(char_height);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_image_text_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  iwidth      [IN]        
 *  iheight     [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_image_text_item(S32 x1, S32 y1, S32 iwidth, S32 iheight, wgui_inline_item *item)
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


/*****************************************************************************
 * FUNCTION
 *  clear_inline_image_text_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_inline_image_text_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_scrolling_text_stop(&wgui_inline_scroll_text);
    inline_scroll_flag = FALSE;
    memset(&wgui_inline_scroll_text, 0, sizeof(wgui_inline_scroll_text));
    clear_inline_item_keys = UI_dummy_function;
    redraw_current_inline_item = UI_dummy_function;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = UI_dummy_inline_item_pen_function;
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_image_text_item_highlight
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  iwidth      [IN]        
 *  iheight     [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_image_text_item_highlight(S32 x1, S32 y1, S32 iwidth, S32 iheight, wgui_inline_item *item)
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

    clear_inline_item_keys = clear_inline_image_text_item;
    redraw_current_inline_item = redraw_image_text_control;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = inline_image_text_pen_handler;
#endif 
    current_image_text_control = i;
    draw_image_text_control(i, x1, y1, iwidth, iheight, i->image1, i->image2, i->image3, i_edit->buffer, 1);

}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_full_screen_imagetext_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_GB__
	extern void GbInuptMethodEnterCategory5(void);
#endif
void handle_inline_full_screen_imagetext_edit(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 input_type;
    wgui_inline_image_text *image_type = (wgui_inline_image_text*) item;

    /* wgui_inline_item_text_edit *i=&wgui_inline_text_edit_items[0];//(wgui_inline_item_text_edit*)image_type->inline_full_text; */
    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) image_type->inline_full_text;
    S32 inputbox_x = 0;
    S32 inputbox_y = (MMI_title_y + MMI_title_height);
    S32 inputbox_width = MMI_content_width;
    S32 inputbox_height = MMI_content_height - MMI_multitap_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
    UI_UNUSED_PARAMETER(width);
    UI_UNUSED_PARAMETER(height);
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(flags);

    if (i->custom_setup_function != NULL)
    {
        i->custom_setup_function();
        return;
    }
#ifdef __MMI_GB__
	GbInuptMethodEnterCategory5();
#elif defined __MMI_T9__
    InuptMethodEnterCategory5();
#elif defined __MMI_ZI__
    ZiInuptMethodEnterCategory5();
#elif defined __MMI_KA__
    KonkaInuptMethodEnterCategory5();
#elif defined __MMI_ITAP__
    ItapInuptMethodEnterCategory5();
#endif 

    gdi_layer_lock_frame_buffer();
    wgui_text_menuitem_reset_scrolling();
    clear_inline_image_text_item();

    if (i->flags & INLINE_ITEM_CHANGED)
    {
        inline_full_screen_edit_changed = 1;
    }
    else
    {
        inline_full_screen_edit_changed = 0;
    }
    wgui_current_inline_item_type = INLINE_ITEM_TYPE_FULL_SCREEN_EDIT;
    current_wgui_inline_text_edit_item = i;

    current_wgui_inline_text_edit_text_p = text_p;

    input_type = i->input_type;
    if (inline_image_text_edit_flag == 0)
    {
        if (!(i->flags & INLINE_ITEM_OPEN))
        {
            gui_strcpy((UI_string_type) wgui_inline_full_screen_text_edit_buffer, (UI_string_type) i->buffer);
            history_buffer = NULL;
        }
    }
    inline_image_text_edit_flag = 1;

    i->flags &= ~INLINE_ITEM_OPEN;
    inline_full_screen_edit_saved_title = MMI_title_string;
    inline_full_screen_edit_saved_title_icon = MMI_title_icon;
    inline_full_screen_edit_saved_LSK_label = MMI_softkeys[MMI_LEFT_SOFTKEY].text;
    inline_full_screen_edit_saved_RSK_label = MMI_softkeys[MMI_RIGHT_SOFTKEY].text;
    inline_full_screen_edit_saved_LSK_icon = MMI_softkeys[MMI_LEFT_SOFTKEY].normal_up_icon;
    inline_full_screen_edit_saved_RSK_icon = MMI_softkeys[MMI_RIGHT_SOFTKEY].normal_up_icon;
    change_left_softkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    change_right_softkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    clear_key_handlers();
    change_left_softkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    change_right_softkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();
    MMI_title_string = get_string(i->title);
    MMI_title_icon = get_image(i->title_icon);
    register_hide_multitap(category5_hide_multitap);
    wgui_inputbox_validation_callback = i->validation_callback;
    wgui_setup_inputbox(
        inputbox_x,
        inputbox_y,
        inputbox_width,
        inputbox_height,
        wgui_inline_full_screen_text_edit_buffer,
        i->buffer_size,
        INLINE_FULL_SCREEN_EDIT_HISTORY_ID,
        get_string(STR_GLOBAL_BACK),
        get_image(IMG_GLOBAL_BACK),
        (S16) input_type,
        history_buffer,
        1);
    inputbox_height = MMI_content_height - MMI_multitap_height - wgui_inputbox_information_bar_height;
    if (MMI_multiline_inputbox.height > inputbox_height)
    {
        gui_resize_multi_line_input_box(&MMI_multiline_inputbox, MMI_multiline_inputbox.width, inputbox_height);
    }

     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
    dm_get_scr_bg_image(
        &inline_edit_scr_bg_ID,
        &inline_edit_scr_bg_filename,
        &inline_edit_scr_bg_x,
        &inline_edit_scr_bg_y,
        &inline_edit_scr_bg_opacity);
    dm_set_scr_bg_image(inline_edit_scr_bg_ID, inline_edit_scr_bg_filename, inline_edit_scr_bg_x, inline_edit_scr_bg_y, MMI_EDITOR_BG_OPACITY);  
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
     

    register_multiline_inputbox_input_callback(wgui_inline_edit_handle_multiline_inputbox_input);
    wgui_inputbox_RSK_function = handle_inline_full_screen_edit_cancel;
    redraw_current_inline_item = redraw_inline_full_screen_edit;
    set_left_softkey_function(handle_inline_full_screen_edit_complete, KEY_EVENT_UP);
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_set_inputbox_LSK_Function(handle_inline_full_screen_edit_complete);
#endif 

    gdi_layer_unlock_frame_buffer();
    redraw_inline_full_screen_edit();
    complete_inline_item_edit = handle_inline_full_screen_edit_close;

}

/* / ATTACHMENT CONTROL//////////////////// */

#define ATTACHMENT_IMAGE_ITEM_HIGHLIGHTED                   1
#define ATTACHMENT_IMAGE_ITEM_UNHIGHLIGHTED                 0
#define ATTACHMENT_IMAGE_ITEM_ICON_MARGIN    4  /* Margin Between Icon of Inline item and start of Image Attachment Control */
#define ATTACHMENT_IMAGE_ICON_MARGIN         2  /* Margin Between Attachment Images in Image Attachment Control */

#define MAX_INLINE_IMAGE_ATTACHMENT_ITEMS 2

/* 0 ~ (MAX_INLINE_IMAGE_ATTACHMENT_ITEMS-1) inline items containing image attachments controls */
static wgui_inline_image_attachment wgui_inline_image_attachment_items[MAX_INLINE_IMAGE_ATTACHMENT_ITEMS];
static S32 n_inline_attach_items = 0;

/* 0 ~ (MAX_IMAGES_IN_INLINE_ATTACHMENT-1)  images inside current image attachment control */
static S32 n_images_in_inline_attach_item = 0;

static wgui_inline_image_attachment *current_attach_control = NULL;
static MMI_BOOL attach_disable_flag = MMI_FALSE;

extern void show_attachment_images(
                S32 x1,
                S32 y1,
                S32 iwidth,
                S32 iheight,
                wgui_inline_image_attachment *image_control,
                U8);
extern void handle_attachment_image(
                S32 x,
                S32 y,
                S32 width,
                S32 height,
                S32 key_code,
                S32 key_event,
                U8 **text_p,
                void *item,
                U32 flags,
                U8 *history_buffer);
#ifdef __MMI_TOUCH_SCREEN__
extern BOOL inline_image_attachment_pen_handler(
                mmi_pen_event_type_enum pen_event,
                S16 x,
                S16 y,
                gui_inline_item_pen_enum *item_event);
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  clear_inline_image_attachment_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_inline_image_attachment_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* FIXME. reset current_attach_control? */
    clear_inline_item_keys = UI_dummy_function;
    redraw_current_inline_item = UI_dummy_function;
    ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = UI_dummy_inline_item_pen_function;
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  redraw_image_attachment_control
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_image_attachment_control(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_image_attachment *attach_item = current_attach_control;

#ifdef __MMI_TOUCH_SCREEN__
    S32 x1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    if (attach_disable_flag)
    {
        x1 = attach_item->control_x1 + 17;
    }
    else
    {
        x1 = attach_item->control_x1;
    }
    gui_draw_filled_area(
        x1,
        attach_item->control_y1,
        attach_item->control_x1 + attach_item->control_width - 1,
        attach_item->control_y1 + attach_item->control_height - 1,
        wgui_inline_select_fixed_text_menuitem_theme.selected_filler);
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(
        x1,
        attach_item->control_y1,
        attach_item->control_x1 + attach_item->control_width - 1,
        attach_item->control_y1 + attach_item->control_height - 1);
#endif /* __MMI_TOUCH_SCREEN__ */ 
    show_attachment_images(
        attach_item->control_x1,
        attach_item->control_y1,
        attach_item->control_width,
        attach_item->control_height,
        attach_item,
        ATTACHMENT_IMAGE_ITEM_HIGHLIGHTED);
}


/*****************************************************************************
 * FUNCTION
 *  RegisterAttachmentLskFunction
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterAttachmentLskFunction(InlineItem *item, void (*f) (PU8 image, UI_string_type str))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_image_attachment *attach_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (f)
    {
        attach_item = (wgui_inline_image_attachment*) item->item;
        attach_item->lsk_function = f;
    }
}


/*****************************************************************************
 * FUNCTION
 *  RegisterAttachmentRskFunction
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterAttachmentRskFunction(InlineItem *item, void (*f) (PU8 image, UI_string_type str))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_image_attachment *attach_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (f)
    {
        attach_item = (wgui_inline_image_attachment*) item->item;
        attach_item->rsk_function = f;
    }
}


/*****************************************************************************
 * FUNCTION
 *  SetCurrentHighlightedAttachment
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [IN]
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetHighlightedAttachment(wgui_inline_item *inline_item, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	wgui_inline_image_attachment *image = (wgui_inline_image_attachment*)inline_item->item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    
	image->highlited_image = (S16) index;
}


/*****************************************************************************
 * FUNCTION
 *  GetCurrentHighlightedAttachment
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 GetCurrentHighlightedAttachment(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("current highlighted attachment is %d", current_attach_control->highlited_image));
    return current_attach_control->highlited_image;
}

/* Right arrow key handler */


/*****************************************************************************
 * FUNCTION
 *  attach_control_right_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void attach_control_right_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 temp_x1 = 0;

    wgui_inline_image_attachment *image = current_attach_control;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    image->highlited_image++;
    if (image->highlited_image >= n_images_in_inline_attach_item)
    {
        image->highlited_image = 0;
    }
    if ( image->image_change_cb )
    {
        image->image_change_cb(image->highlited_image);
    }

    if (attach_disable_flag)
    {
        temp_x1 = image->control_x1 + 17;
    }
    else
    {
        temp_x1 = image->control_x1;
    }

    gui_draw_filled_area(
        temp_x1,
        image->control_y1,
        image->control_x1 + image->control_width - 1,
        image->control_y1 + image->control_height - 1,
        wgui_inline_select_fixed_text_menuitem_theme.selected_filler);

    gdi_layer_unlock_frame_buffer();

    show_attachment_images(
        image->control_x1,
        image->control_y1,
        image->control_width,
        image->control_height,
        image,
        ATTACHMENT_IMAGE_ITEM_HIGHLIGHTED);
}

/* Left arrow key handler */


/*****************************************************************************
 * FUNCTION
 *  attach_control_left_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void attach_control_left_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 temp_x1 = 0;

    wgui_inline_image_attachment *image = current_attach_control;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    if (image->highlited_image == 0)
    {
        image->highlited_image = (S16) (n_images_in_inline_attach_item - 1);
    }
    else
    {
        image->highlited_image--;
    }
    if ( image->image_change_cb )
    {
        image->image_change_cb(image->highlited_image);
    }

    if (attach_disable_flag)
    {
        temp_x1 = image->control_x1 + 17;
    }
    else
    {
        temp_x1 = image->control_x1;
    }

    gui_draw_filled_area(
        temp_x1,
        image->control_y1,
        image->control_x1 + image->control_width - 1,
        image->control_y1 + image->control_height - 1,
        wgui_inline_select_fixed_text_menuitem_theme.selected_filler);

    gdi_layer_unlock_frame_buffer();

    show_attachment_images(
        image->control_x1,
        image->control_y1,
        image->control_width,
        image->control_height,
        image,
        ATTACHMENT_IMAGE_ITEM_HIGHLIGHTED);
}


/*****************************************************************************
 * FUNCTION
 *  ClearAllAttachmentImages
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                    [?]         
 *  attachments_present     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ClearAllAttachmentImages(InlineItem *item, S32 attachments_present)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;
    wgui_inline_image_attachment *attach_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    attach_item = (wgui_inline_image_attachment*) item->item;

    for (i = 0; i < attachments_present; i++)
    {
        attach_item->attach_image[i].image1 = 0;
        gui_strcpy((UI_string_type) (attach_item->attach_image[i].image_file_name), (UI_string_type) L"\0");
        attach_item->attach_image[i].type_of_image = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  ClearAttachmentImage
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ClearAttachmentImage(InlineItem *item, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;
    wgui_inline_image_attachment *attach_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    attach_item = current_attach_control;
    attach_item = (wgui_inline_image_attachment*) item->item;

    for (i = index; i < MAX_IMAGES_IN_INLINE_ATTACHMENT; i++)
    {
        if (i == MAX_IMAGES_IN_INLINE_ATTACHMENT - 1)
        {
            attach_item->attach_image[i].image1 = 0;
            gui_strcpy((UI_string_type) (attach_item->attach_image[i].image_file_name), (UI_string_type) L"\0");
            attach_item->attach_image[i].type_of_image = 0;
        }
        else
        {
            attach_item->attach_image[i].image1 = attach_item->attach_image[i + 1].image1;
            gui_strcpy(
                (UI_string_type) (attach_item->attach_image[i].image_file_name),
                (UI_string_type) (attach_item->attach_image[i + 1].image_file_name));
            attach_item->attach_image[i].type_of_image = attach_item->attach_image[i + 1].type_of_image;
        }
    }
}

/* Add an image inside an image attachment inline item. 
   This function should be called after SetInlineItemImageAttachment(). */
/* FIXME. we should change function prototype to pass the number of images as a parameter
   instead of using MAX_IMAGES_IN_INLINE_ATTACHMENT */


/*****************************************************************************
 * FUNCTION
 *  AddEmailImageAttachmentUI
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                [?]     
 *  image_details       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void AddEmailImageAttachmentUI(InlineItem *item, wgui_inline_images_detail *image_details)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_image_attachment *attach_item;
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT((item->flags & 0xFF) == INLINE_ITEM_ATTACH_IMAGE);

    attach_item = (wgui_inline_image_attachment*) item->item;

    for (i = 0; i < MAX_IMAGES_IN_INLINE_ATTACHMENT; i++)
    {
        if (image_details[i].image1)
        {
            attach_item->attach_image[i].image1 = image_details[i].image1;
            attach_item->attach_image[i].type_of_image = image_details[i].type_of_image;
            pfnUnicodeStrcpy(
                (PS8) attach_item->attach_image[i].image_file_name,
                (PS8) image_details[i].image_file_name);
        }
    }
}

/* Create an inline item containing image attachment */


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemImageAttachment
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                [?]         
 *  image1              [IN]        
 *  image2              [IN]        
 *  image3              [IN]        
 *  title               [IN]        
 *  title_icon          [IN]        
 *  highlight_image     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemImageAttachment(
        InlineItem *item,
        PU8 image1,
        PU8 image2,
        PU8 image3,
        U16 title,
        U16 title_icon,
        U8 highlight_image)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_image_attachment *attach_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_inline_attach_items >= MAX_INLINE_IMAGE_ATTACHMENT_ITEMS)
    {
        MMI_DBG_ASSERT(0);
        return;
    }

    item->item = (void*)&wgui_inline_image_attachment_items[n_inline_attach_items];
    item->flags = INLINE_ITEM_ATTACH_IMAGE;
    item->show_img_flag = FALSE;

    n_inline_attach_items++;

    attach_item = (wgui_inline_image_attachment*) item->item;

    attach_item->title = title;
    attach_item->title_icon = title_icon;
    attach_item->highlited_image = highlight_image;
    attach_item->flags = 0;
    attach_item->image1 = image1;
    attach_item->image2 = image2;
    attach_item->image3 = image3;
    item->handler = handle_attachment_image;
#ifdef __MMI_TOUCH_SCREEN__
    attach_item->highlight_changed = 0;
    memset(attach_item->image_left_top, 0, sizeof(attach_item->image_left_top));
    memset(attach_item->image_right_bottom, 0, sizeof(attach_item->image_right_bottom));
#endif /* __MMI_TOUCH_SCREEN__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  ShowImageAttach
 * DESCRIPTION
 *  
 * PARAMETERS
 *  image       [IN]        
 *  str         [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ShowImageAttach(PU8 image, UI_string_type str)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 width = 0, height = 0, img_x = 0, img_y = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    clear_screen();

    change_left_softkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    change_right_softkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;

    clear_key_handlers();

    clear_left_softkey();
    clear_right_softkey();

    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();

    change_left_softkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    change_right_softkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

    MMI_title_string = (UI_string_type) str;
    MMI_title_icon = 0;

    gui_measure_image(image, &width, &height);

    img_x = (UI_device_width >> 1) - (width >> 1);
    img_y = (MMI_title_y + MMI_title_height) + 1;

    img_y = img_y + ((MMI_CONTENT_HEIGHT >> 1) - (height >> 1));

    draw_title();
    gui_show_image(img_x, img_y, image);

    gdi_layer_unlock_frame_buffer();

    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  handle_attachment_image
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  iwidth              [IN]        
 *  iheight             [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_attachment_image(
        S32 x,
        S32 y,
        S32 iwidth,
        S32 iheight,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 width = 0, height = 0;
    wgui_inline_image_attachment *image_attach = (wgui_inline_image_attachment*) item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
    UI_UNUSED_PARAMETER(width);
    UI_UNUSED_PARAMETER(height);
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(flags);

    image_attach->lsk_function(
                    image_attach->attach_image[image_attach->highlited_image].image1,
                    (UI_string_type) image_attach->attach_image[image_attach->highlited_image]. image_file_name);
}


/*****************************************************************************
 * FUNCTION
 *  register_inline_image_attach_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_inline_image_attach_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(attach_control_right_arrow, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(attach_control_left_arrow, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  show_attachment_images
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1                  [IN]        
 *  y1                  [IN]        
 *  iwidth              [IN]        
 *  iheight             [IN]        
 *  image_control       [?]         
 *  highlight           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void show_attachment_images(
        S32 x1,
        S32 y1,
        S32 iwidth,
        S32 iheight,
        wgui_inline_image_attachment *image_control,
        U8 highlight)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S32 width = 0, height = 0, temp_x = x1 + 1, index = 0, img_y1 = 0;
    color_t blue_col = gui_color(0, 0, 255);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (highlight)
    {
        gdi_layer_lock_frame_buffer();
    }

    gdi_layer_reset_clip();
    gui_reset_text_clip();

    if (!r2lMMIFlag)
    {
        if (image_control->image1)
        {
            gui_measure_image((PU8) image_control->image1, &width, &height);
            img_y1 = (iheight >> 1) - (height >> 1);
            gui_show_transparent_image(temp_x, (y1 + img_y1), image_control->image1, 0);
            temp_x = x1 + width + ATTACHMENT_IMAGE_ITEM_ICON_MARGIN;
        }

        if (image_control->image2)
        {
            gui_measure_image(image_control->image2, &width, &height);
            img_y1 = (iheight >> 1) - (height >> 1);
            gui_show_transparent_image(temp_x, (y1 + img_y1), image_control->image2, 0);
            temp_x += width + ATTACHMENT_IMAGE_ICON_MARGIN;

        }

      /** Show attachment images */
        n_images_in_inline_attach_item = 0;
        for (index = 0; index < MAX_IMAGES_IN_INLINE_ATTACHMENT; index++)
        {
            if (image_control->attach_image[index].image1)
            {
                n_images_in_inline_attach_item++;
                gui_measure_image((PU8) image_control->attach_image[index].image1, &width, &height);
                img_y1 = (iheight >> 1) - (height >> 1);

                if ((image_control->highlited_image == index) && (highlight == ATTACHMENT_IMAGE_ITEM_HIGHLIGHTED))
                {
                    gui_fill_rectangle(temp_x - 1, y1 + img_y1 - 1, (temp_x + width), (y1 + img_y1 + height), blue_col);
                }

                gui_show_transparent_image(temp_x, (y1 + img_y1), image_control->attach_image[index].image1, 0);

            #ifdef __MMI_TOUCH_SCREEN__
                image_control->image_left_top[index].x = temp_x;
                image_control->image_left_top[index].y = y1 + img_y1;
                image_control->image_right_bottom[index].x = temp_x + width - 1;
                image_control->image_right_bottom[index].y = y1 + img_y1 + height - 1;
            #endif /* __MMI_TOUCH_SCREEN__ */ 

                temp_x = temp_x + width + ATTACHMENT_IMAGE_ICON_MARGIN;
            }
        }

      /********/

        if (image_control->image3)
        {
            gui_measure_image(image_control->image3, &width, &height);
            img_y1 = (MMI_INLINE_EDIT_MENUITEM_HEIGHT >> 1) - (height >> 1);
            /* temp_x = (UI_device_width - width - 8); */
            temp_x = (x1 + 1) + iwidth - (width + ATTACHMENT_IMAGE_ICON_MARGIN);
            gui_show_transparent_image(temp_x, (y1 + img_y1), image_control->image3, 0);
        }

    }
    else
    {
        temp_x = UI_device_width - 1;
        if (image_control->image1)
        {
            gui_measure_image((PU8) image_control->image1, &width, &height);
            img_y1 = (iheight >> 1) - (height >> 1);
            temp_x -= width;
            gui_show_transparent_image(temp_x, (y1 + img_y1), image_control->image1, 0);
        }

        if (image_control->image3)
        {
            gui_measure_image(image_control->image3, &width, &height);
            img_y1 = (iheight >> 1) - (height >> 1);
            temp_x -= (width + ATTACHMENT_IMAGE_ICON_MARGIN);
            gui_show_transparent_image(temp_x, (y1 + img_y1), image_control->image3, 0);
        }

      /** Show attachment images */
        n_images_in_inline_attach_item = 0;
        for (index = MAX_IMAGES_IN_INLINE_ATTACHMENT - 1; index >= 0; index--)
        {
            if (image_control->attach_image[index].image1)
            {
                n_images_in_inline_attach_item++;
                gui_measure_image((PU8) image_control->attach_image[index].image1, &width, &height);
                img_y1 = (iheight >> 1) - (height >> 1);
                temp_x -= (width + ATTACHMENT_IMAGE_ICON_MARGIN);
                if ((image_control->highlited_image == index) && (highlight == ATTACHMENT_IMAGE_ITEM_HIGHLIGHTED))
                {
                    gui_fill_rectangle(
                        (temp_x - 1),
                        y1 + img_y1 - 1,
                        (temp_x + width),
                        (y1 + img_y1 + height),
                        blue_col);
                }

                gui_show_transparent_image(temp_x, (y1 + img_y1), image_control->attach_image[index].image1, 0);

            #ifdef __MMI_TOUCH_SCREEN__
                image_control->image_left_top[index].x = temp_x;
                image_control->image_left_top[index].y = y1 + img_y1;
                image_control->image_right_bottom[index].x = temp_x + width - 1;
                image_control->image_right_bottom[index].y = y1 + img_y1 + height - 1;
            #endif /* __MMI_TOUCH_SCREEN__ */ 
            }
        }

        if (image_control->image2)
        {
            gui_measure_image(image_control->image2, &width, &height);
            img_y1 = (MMI_INLINE_EDIT_MENUITEM_HEIGHT >> 1) - (height >> 1);
            temp_x = (UI_device_width - 1) - iwidth + ATTACHMENT_IMAGE_ICON_MARGIN;
            gui_show_transparent_image(temp_x, (y1 + img_y1), image_control->image2, 0);
        }
    }

    if (highlight)
    {
        gdi_layer_unlock_frame_buffer();
        gdi_layer_blt_previous(1, y1, (x1 + iwidth - 1), (y1 + iheight - 1));
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_image_attachment_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  iwidth      [IN]        
 *  iheight     [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_image_attachment_item(S32 x1, S32 y1, S32 iwidth, S32 iheight, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_image_attachment *image = (wgui_inline_image_attachment*) item->item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((item->flags & INLINE_ITEM_DISABLE_HIGHLIGHT))
    {
        attach_disable_flag = MMI_TRUE;
    }
    else
    {
        attach_disable_flag = MMI_FALSE;
    }

    image->highlited_image = 0;
    if ( image->image_change_cb )
    {
        image->image_change_cb(0);
    }
    show_attachment_images(x1, y1, iwidth, iheight, image, ATTACHMENT_IMAGE_ITEM_UNHIGHLIGHTED);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_image_attachment_item_highlight
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  iwidth      [IN]        
 *  iheight     [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_image_attachment_item_highlight(S32 x1, S32 y1, S32 iwidth, S32 iheight, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_image_attachment *image = (wgui_inline_image_attachment*) item->item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    image->control_x1 = (U8) x1;
    image->control_y1 = (U8) y1;
    image->control_width = (U8) iwidth;
    image->control_height = (U8) iheight;

    current_attach_control = image;
    register_inline_image_attach_keys();

    clear_inline_item_keys = clear_inline_image_attachment_item;
    redraw_current_inline_item = redraw_image_attachment_control;

    register_inline_image_attach_keys();
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = inline_image_attachment_pen_handler;
#endif 

    show_attachment_images(x1, y1, iwidth, iheight, image, ATTACHMENT_IMAGE_ITEM_HIGHLIGHTED);
}

/*****************************************************************************
* FUNCTION
*	 RegisterAttachmentHighlightedFunction()
* DESCRIPTION
*   Register highlighted item callback function
* PARAMETERS
*	 item		IN			inline item
*	 f			IN			callback function
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void RegisterAttachmentHighlightedFunction (InlineItem * item, void (*f)(S32 index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_image_attachment *attach_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (f)
    {
        attach_item = (wgui_inline_image_attachment *) item->item;
        attach_item->image_change_cb= f;
    }
}

#ifdef __MMI_TOUCH_SCREEN__

/*
 * If success, *item_index >= 0 and return MMI_TRUE
 * * Otherwise, *item_index < 0 and return MMI_FALSE
 */


/*****************************************************************************
 * FUNCTION
 *  gui_image_attachment_translate_pen_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x               [IN]        
 *  y               [IN]        
 *  item_index      [?]         
 * RETURNS
 *  
 *****************************************************************************/
static BOOL gui_image_attachment_translate_pen_position(S16 x, S16 y, S32 *item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 index;
    S32 x1, y1, x2, y2;
    wgui_inline_image_attachment *image_control;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_DBG_ASSERT(current_attach_control);

    image_control = current_attach_control;

    *item_index = -1;

    for (index = 0; index < n_images_in_inline_attach_item; index++)
    {
        if (image_control->attach_image[index].image1)
        {
            x1 = image_control->image_left_top[index].x;
            y1 = image_control->image_left_top[index].y;
            x2 = image_control->image_right_bottom[index].x;
            y2 = image_control->image_right_bottom[index].y;

            if (x >= x1 && x <= x2 && y >= y1 && y <= y2)
            {
                *item_index = index;
                break;
            }
        }
    }

    if (*item_index < 0)
    {
        return MMI_FALSE;
    }
    else
    {
        return MMI_TRUE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  inline_image_attachment_pen_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pen_event       [IN]        
 *  x               [IN]        
 *  y               [IN]        
 *  item_event      [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL inline_image_attachment_pen_handler(
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_inline_item_pen_enum *item_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret;
    S32 x1, y1, x2, y2;
    wgui_inline_image_attachment *image_control;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_DBG_ASSERT(current_attach_control);

    image_control = current_attach_control;

    x1 = image_control->control_x1;
    y1 = image_control->control_y1;
    x2 = x1 + image_control->control_width;
    y2 = y1 + image_control->control_height;

    ret = MMI_TRUE;
    *item_event = GUI_INLINE_ITEM_PEN_NONE;

    switch (pen_event)
    {
        case MMI_PEN_EVENT_DOWN:
        {
            if (PEN_CHECK_BOUND(x, y, x1, y1, x2, y2))
            {
                S32 index;

                if (gui_image_attachment_translate_pen_position(x, y, &index))
                {
                    image_control->highlited_image = index;
                    if ( image_control->image_change_cb )
                    {
                        image_control->image_change_cb(image_control->highlited_image);
                    }
                    *item_event = GUI_INLINE_ITEM_PEN_NEED_REDRAW_ITEM;
                }
                image_control->highlight_changed = 0;
            }
            else
            {
                ret = MMI_FALSE;
            }
        }
            break;

        case MMI_PEN_EVENT_MOVE:
        {
            S32 index;

            gui_image_attachment_translate_pen_position(x, y, &index);
            if (index != image_control->highlited_image)
            {
                image_control->highlight_changed = 1;
            }
        }
            break;

        case MMI_PEN_EVENT_UP:
        {
            S32 index;

            gui_image_attachment_translate_pen_position(x, y, &index);
            if (index == image_control->highlited_image && !image_control->highlight_changed)
            {
                *item_event = GUI_INLINE_ITEM_PEN_SELECTED;
            }
        }
            break;

        case MMI_PEN_EVENT_LONG_TAP:
            /* Do Nothing */
            break;

        case MMI_PEN_EVENT_REPEAT:
            /* Do Nothing */
            break;

        case MMI_PEN_EVENT_ABORT:
            /* Do Nothing */
            break;

        default:
            MMI_DBG_ASSERT(0);
    }

    return ret;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 

#endif /* defined __MMI_PLUTO_GPRS__ */ 

/************************** Image Text Control End ********/


/*****************************************************************************
 * FUNCTION
 *  inline_fixed_list_goto_previous_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_fixed_list_goto_previous_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* FIXME. register as complete_inline_item_edit()? */
    if (!(MMI_multiline_inputbox.flags & UI_MULTI_LINE_INPUT_BOX_VIEW_MODE) &&
        (MMI_multiline_inputbox.flags & UI_MULTI_LINE_INPUT_BOX_INSIDE_INLINE_MENU))
    {
        handle_inline_multi_line_edit_complete();
    }
    wgui_inline_edit_arrow_key_flag = CATEGORY57_UP_ARROW_KEY_FLAG;
    clear_inline_item_keys();
    register_inline_fixed_list_keys();
#if 0   /* defined(__MMI_TOUCH_SCREEN__) */
#else /* 0 */ 
    fixed_list_goto_previous_item();
#if defined(__MMI_TOUCH_SCREEN__)
     
    if (GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY != MMI_virtual_keyboard.lang_type)
    {
        mmi_pen_editor_clear_and_show_virtual_keyboard_area();
    }
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
#endif /* 0 */ 
}


/*****************************************************************************
 * FUNCTION
 *  inline_fixed_list_goto_next_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_fixed_list_goto_next_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_edit_arrow_key_flag = CATEGORY57_DOWN_ARROW_KEY_FLAG;
    clear_inline_item_keys();
    register_inline_fixed_list_keys();
#if 0   /* defined(__MMI_TOUCH_SCREEN__) */
#else /* 0 */ 
    fixed_list_goto_next_item();
#if defined(__MMI_TOUCH_SCREEN__)
     
    if (GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY != MMI_virtual_keyboard.lang_type)
    {
        mmi_pen_editor_clear_and_show_virtual_keyboard_area();
    }
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
#endif /* 0 */ 
}


/*****************************************************************************
 * FUNCTION
 *  inline_fixed_list_goto_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index           [IN]        
 *  backward        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_fixed_list_goto_item(S32 index, BOOL backward)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* FIXME. register as complete_inline_item_edit()? */
    if (!(MMI_multiline_inputbox.flags & UI_MULTI_LINE_INPUT_BOX_VIEW_MODE) &&
        (MMI_multiline_inputbox.flags & UI_MULTI_LINE_INPUT_BOX_INSIDE_INLINE_MENU))
    {
        handle_inline_multi_line_edit_complete();
    }

    if (backward)
    {
        wgui_inline_edit_arrow_key_flag = CATEGORY57_UP_ARROW_KEY_FLAG;
    }
    else
    {
        wgui_inline_edit_arrow_key_flag = CATEGORY57_DOWN_ARROW_KEY_FLAG;
    }
    clear_inline_item_keys();
    register_inline_fixed_list_keys();
#if 0   /* defined(__MMI_TOUCH_SCREEN__) */
#else /* 0 */ 
    fixed_list_goto_item(index);
#if defined(__MMI_TOUCH_SCREEN__)
     
    if (GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY != MMI_virtual_keyboard.lang_type)
    {
        mmi_pen_editor_clear_and_show_virtual_keyboard_area();
    }
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
#endif /* 0 */ 
}


/*****************************************************************************
 * FUNCTION
 *  inline_fixed_list_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  vkey_code       [IN]        
 *  key_state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_fixed_list_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 37:
                break;
            case 39:
                break;
            case 38:
                inline_fixed_list_goto_previous_item();
                break;
            case 40:
                inline_fixed_list_goto_next_item();
                break;
            case 36:
                clear_inline_item_keys();
                fixed_list_goto_home();
                break;
            case 35:
                clear_inline_item_keys();
                fixed_list_goto_end();
                break;
            case 33:
                clear_inline_item_keys();
                fixed_list_goto_previous_page();
                break;
            case 34:
                clear_inline_item_keys();
                fixed_list_goto_next_page();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}


/*****************************************************************************
 * FUNCTION
 *  register_inline_fixed_list_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_inline_fixed_list_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_keyboard_key_handler(inline_fixed_list_key_handler);
    SetKeyHandler(inline_fixed_list_goto_previous_item, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_fixed_list_goto_next_item, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_fixed_list_goto_previous_item, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(inline_fixed_list_goto_next_item, KEY_VOL_DOWN, KEY_EVENT_DOWN);
}

/* Changes to allow Back/Done key and Clear key in case of Inline edit  */

 


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_edit_get_RSK_string
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
UI_string_type wgui_inline_edit_get_RSK_string(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_inline_list_menu_changed)
    {
        if (wgui_inline_list_menu_disable_done == 1)
        {
            return ((UI_string_type) get_string(STR_GLOBAL_BACK));
        }
        else if (wgui_inline_list_menu_disable_done == 2)
        {
            return ((UI_string_type) get_string(STR_GLOBAL_CLEAR));
        }
        else
        {
            return ((UI_string_type) get_string(WGUI_CATEGORY_DONE_STRING_ID));
        }

    }
    else
    {
        return ((UI_string_type) get_string(STR_GLOBAL_BACK));
    }
}


/*****************************************************************************
 * FUNCTION
 *  execute_wgui_inline_edit_DONE_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void execute_wgui_inline_edit_DONE_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    complete_inline_item_edit();
    MMI_multi_line_inputbox_present = 0;
    if (wgui_inline_edit_DONE_function != NULL)
    {
        wgui_inline_edit_DONE_function();
    }
}


/*****************************************************************************
 * FUNCTION
 *  execute_wgui_inline_edit_BACK_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void execute_wgui_inline_edit_BACK_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_inline_edit_BACK_function != NULL)
    {
        MMI_multi_line_inputbox_present = 0;
        wgui_inline_edit_BACK_function();
    }
}


/*****************************************************************************
 * FUNCTION
 *  execute_wgui_inline_edit_RSK_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void execute_wgui_inline_edit_RSK_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_inline_list_menu_changed)
    {
        if (wgui_inline_list_menu_disable_done)
        {
            execute_wgui_inline_edit_BACK_function();
        }
        else
        {
            execute_wgui_inline_edit_DONE_function();
        }
    }
    else
    {
        execute_wgui_inline_edit_BACK_function();
    }
}


/*****************************************************************************
 * FUNCTION
 *  execute_wgui_inline_edit_LSK_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void execute_wgui_inline_edit_LSK_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    complete_inline_item_edit();
    if (wgui_inline_edit_LSK_function != NULL)
    {
        wgui_inline_edit_LSK_function();
    }
}


/*****************************************************************************
 * FUNCTION
 *  set_wgui_inline_list_menu_changed
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_wgui_inline_list_menu_changed(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    if (inline_item_changed_handler != NULL)
    {
        inline_item_changed_handler((U16) wgui_inline_item_highlighted_index);
    }
     
    if (wgui_inline_list_menu_disable_done)
    {
        return;
    }
    if (wgui_inline_list_menu_changed)
    {
        return;
    }
    wgui_inline_list_menu_changed = 1;
    set_right_softkey_label(wgui_inline_edit_get_RSK_string());
    set_right_softkey_icon(NULL);
    register_right_softkey_handler();
    set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
    redraw_right_softkey();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_edit_register_BACK_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_edit_register_BACK_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_right_softkey_function(execute_wgui_inline_edit_BACK_function, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_edit_register_ckey_BACK_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_edit_register_ckey_BACK_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(execute_wgui_inline_edit_BACK_function, KEY_CLEAR, KEY_EVENT_UP);
}

 
#ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
extern is_button_bar_shuffled;
#endif 
 


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_text_edit_set_RSK_BACK_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_text_edit_set_RSK_BACK_function(void)
{   /* reset_right_softkey();           */
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_inline_list_menu_disable_done)
    {
    #if defined(__MMI_WITH_C_KEY__)
        SetKeyHandler(UI_dummy_function, KEY_CLEAR, KEY_EVENT_DOWN);
        SetKeyHandler(wgui_inline_edit_register_ckey_BACK_function, KEY_CLEAR, KEY_EVENT_UP);
    #else /* defined(__MMI_WITH_C_KEY__) */ 
        set_right_softkey_label((UI_string_type) get_string(STR_GLOBAL_BACK));
        set_right_softkey_icon(NULL);
        set_right_softkey_function(wgui_inline_edit_register_BACK_function, KEY_EVENT_UP);
        set_right_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
    #endif /* defined(__MMI_WITH_C_KEY__) */ 
    }
    else
    {
        reset_right_softkey();
        set_right_softkey_label(NULL);
        set_right_softkey_icon(NULL);
         
    #ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
        is_button_bar_shuffled = 1;
        gui_call_shuffle(); /* To shuffle the key as button up is not called. */
    #endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
         
    }
    redraw_right_softkey();
}

/* History IDs for inline items  */

//#define INLINE_TEXT_EDIT_HISTORY_ID              1
//#define INLINE_SELECT_HISTORY_ID              3
//#define INLINE_USER_DEFINED_SELECT_HISTORY_ID    4
//#define INLINE_DATE_HISTORY_ID                5
//#define INLINE_TIME_HISTORY_ID                6
//#define INLINE_DOW_SELECT_HISTORY_ID          7
//#define INLINE_TIME_PERIOD_HISTORY_ID            8
//#define INLINE_IP4_HISTORY_ID                 9
//#define INLINE_FULL_SCREEN_EDIT_HISTORY_ID       10

/*----------------------------------------------------------------------------
   "Display only" inline item implementation
----------------------------------------------------------------------------*/


/*****************************************************************************
 * FUNCTION
 *  handle_inline_display_only
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_display_only(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
    UI_UNUSED_PARAMETER(height);
    UI_UNUSED_PARAMETER(width);
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(text_p);
    UI_UNUSED_PARAMETER(item);
    UI_UNUSED_PARAMETER(flags);
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemDisplayOnly
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 *  text_p      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemDisplayOnly(InlineItem *item, U8 *text_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->item = NULL;
    item->text_p = text_p;
    item->handler = handle_inline_display_only;
    item->flags = INLINE_ITEM_TYPE_DISPLAY_ONLY;
}


/*****************************************************************************
 * FUNCTION
 *  ReConfigureInlineItemDisplayOnly
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 *  text_p      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void ReConfigureInlineItemDisplayOnly(InlineItem *item, U8 *text_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((item->flags & 0xff) != INLINE_ITEM_TYPE_DISPLAY_ONLY)
    {
        return;
    }
    item->text_p = text_p;
}

/*----------------------------------------------------------------------------
   "Caption" inline item implementation
----------------------------------------------------------------------------*/


/*****************************************************************************
 * FUNCTION
 *  handle_inline_caption
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_caption(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
    UI_UNUSED_PARAMETER(height);
    UI_UNUSED_PARAMETER(width);
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(text_p);
    UI_UNUSED_PARAMETER(item);
    UI_UNUSED_PARAMETER(flags);
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemCaption
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 *  text_p      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemCaption(InlineItem *item, U8 *text_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->item = NULL;
    item->text_p = text_p;
    item->handler = handle_inline_caption;
    item->flags = INLINE_ITEM_TYPE_CAPTION;
}


/*****************************************************************************
 * FUNCTION
 *  ReconfigureInlineItemCaption
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 *  text_p      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void ReconfigureInlineItemCaption(InlineItem *item, U8 *text_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((item->flags & 0xff) != INLINE_ITEM_TYPE_CAPTION)
    {
        return;
    }
    item->text_p = text_p;
}

/*----------------------------------------------------------------------------
   Inline text edit (single line) implementation
----------------------------------------------------------------------------*/

//#define MAX_INLINE_TEXT_EDIT_ITEMS                     10
//#define MAX_INLINE_TEXT_EDIT_BUFFER_SIZE               88

wgui_inline_item_text_edit wgui_inline_text_edit_items[MAX_INLINE_TEXT_EDIT_ITEMS];
U8 wgui_inline_text_edit_buffer[MAX_INLINE_TEXT_EDIT_BUFFER_SIZE];
U8 wgui_inline_full_screen_text_edit_buffer[MAX_INLINE_FULL_SCREEN_TEXT_EDIT_BUFFER_SIZE];
wgui_inline_item_text_edit *current_wgui_inline_text_edit_item;
U8 **current_wgui_inline_text_edit_text_p;
single_line_input_box MMI_inline_singleline_inputbox;
U8 inline_text_edit_mask_buffers[MAX_INLINE_TEXT_EDIT_ITEMS][MAX_INLINE_TEXT_EDIT_BUFFER_SIZE];
UI_character_type inline_text_edit_mask_character = (UI_character_type) '*';
U8 wgui_inline_singleline_inputbox_decimal_flag = 0;
U8 wgui_inline_singleline_inputbox_decimal_input_mode_flag = 0;
extern U8 MMI_current_input_mode;
S32 wgui_inline_singleline_inputbox_buffer_size = 0;

void (*inline_singleline_inputbox_input_callback) (void) = UI_dummy_function;
void (*inline_singleline_inputbox_navigate_callback) (void) = UI_dummy_function;
extern S32(*wgui_inputbox_UCS2_count_function) (void);

#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_INLINE_SINGLELINE_INPUTBOX_FRAME_SKIPPING))
S32 wgui_inline_singleline_inputbox_frame_counter = 0;
#endif 

UI_filled_area temp_inline_edit_background_filler = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
    NULL,
    NULL,
     
#ifdef __MMI_DEFAULT_THEME_3__
    {206, 206, 206, 100},
#else 
    {167, 232, 93, 100},
#endif 
     
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {0, 0, 0, 0},
    0
};

UI_filled_area temp_inline_edit_background_filler2 = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
    NULL,
    NULL,
    {255, 255, 255, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {0, 0, 0, 0},
    0
};

UI_filled_area temp_inline_edit_background_filler3 = {UI_FILLED_AREA_TYPE_COLOR,
    NULL,
    NULL,
    {167, 232, 93, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {0, 0, 0, 0},
    0
};

/* Filler used for focussed inputbox   with boundary  */
UI_single_line_input_box_theme wgui_inline_singleline_inputbox_focussed_theme = { &temp_inline_edit_background_filler,
    &temp_inline_edit_background_filler,
    &temp_inline_edit_background_filler,
    {0, 0, 0, 100},
    {128, 128, 128, 100},
    {0, 0, 0, 100},
    {255, 255, 255, 100},
    {51, 88, 171, 100},
    {255, 0, 0, 100},
    &MMI_default_font,
    1,
#if defined(__PROJECT_GALLITE_C01__) 
    UI_SINGLE_LINE_INPUT_BOX_CENTER_Y | UI_SINGLE_LINE_INPUT_BOX_NO_BORDER,
#else
    UI_SINGLE_LINE_INPUT_BOX_CENTER_Y,
#endif
    '*'
};

/* Filler used for unfocussed inputbox with boundary  */
UI_single_line_input_box_theme wgui_inline_singleline_inputbox_unfocussed_theme =
    { &temp_inline_edit_background_filler2,
    &temp_inline_edit_background_filler2,
    &temp_inline_edit_background_filler2,
    {0, 0, 0, 100},
    {128, 128, 128, 100},
    {0, 0, 0, 100},
    {255, 255, 255, 100},
    {51, 88, 171, 100},
    {255, 0, 0, 100},
    &MMI_default_font,
    1,
    UI_SINGLE_LINE_INPUT_BOX_CENTER_Y,
    '*'
};

/* Filler used for inputbox without boundary */
UI_single_line_input_box_theme wgui_inline_singleline_inputbox_no_boundary_theme =
    { &temp_inline_edit_background_filler3,
    &temp_inline_edit_background_filler3,
    &temp_inline_edit_background_filler3,
    {0, 0, 0, 100},
    {128, 128, 128, 100},
    {0, 0, 0, 100},
    {255, 255, 255, 100},
    {51, 88, 171, 100},
    {255, 0, 0, 100},
    &MMI_default_font,
    1,
    UI_SINGLE_LINE_INPUT_BOX_CENTER_Y,
    '*'
};

void (*inline_text_edit_RSK_function) (void) = NULL;
U8 inline_text_edit_RSK_label_clear = 0;
UI_string_type inline_text_edit_RSK_label_string;
PU8 inline_text_edit_RSK_label_icon;

#define WGUI_INLINE_TEXT_EDIT_MULTITAP_NONE     0
#define WGUI_INLINE_TEXT_EDIT_MULTITAP_STAR     1
#define WGUI_INLINE_TEXT_EDIT_MULTITAP_ANY      2
 
#define WGUI_INLINE_TEXT_EDIT_MULTITAP_ZERO     3
/* CSD end */

U8 wgui_inline_text_edit_active_multitap = WGUI_INLINE_TEXT_EDIT_MULTITAP_NONE;
UI_character_type wgui_inline_edit_phone_number_star_key_string[] = { '*', '+', 'p', 'w', '\0' };
multitap_input wgui_inline_text_edit_multitap_star_key;

 
UI_character_type wgui_inline_edit_phone_number_zero_key_string[] = { '+', 'p', 'w', '\0' };
multitap_input wgui_inline_text_edit_multitap_zero_key;

/* CSD end */

extern void MMI_key_0_down(void);
extern void MMI_key_0_up(void);



/*****************************************************************************
 * FUNCTION
 *  inline_edit_prepare_mask_buffer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  buffer1     [?]     
 *  buffer2     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_prepare_mask_buffer(U8 *buffer1, U8 *buffer2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_character_type c = (UI_character_type) - 1;
    U16 eos = '\0';

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (!UI_STRING_END_OF_STRING_CHARACTER(c))
    {
        UI_STRING_GET_NEXT_CHARACTER(buffer1, c);
        if (UI_STRING_END_OF_STRING_CHARACTER(c))
        {
            UI_STRING_INSERT_CHARACTER(buffer2, eos);
        }
        else
        {
            UI_STRING_INSERT_CHARACTER(buffer2, inline_text_edit_mask_character);
        }
    }
}

 /*
  * void UI_dummy_edit_complete_handler(U8 *buffer,S32 buffer_size)
  * { UI_UNUSED_PARAMETER(buffer);
  * UI_UNUSED_PARAMETER(buffer_size);
  */


/*****************************************************************************
 * FUNCTION
 *  create_inline_edit_singleline_inputbox_set_buffer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  buffer              [IN]        
 *  buffer_length       [IN]        
 *  text_length         [IN]        
 *  edit_position       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void create_inline_edit_singleline_inputbox_set_buffer(
        UI_string_type buffer,
        S32 buffer_length,
        S32 text_length,
        S32 edit_position)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_single_line_input_box_theme *t = current_single_line_input_box_theme;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    current_single_line_input_box_theme = &wgui_inline_singleline_inputbox_focussed_theme;
    inline_singleline_inputbox_input_callback = UI_dummy_function;
    inline_singleline_inputbox_navigate_callback = UI_dummy_function;
#ifdef __UCS2_ENCODING
    gui_create_single_line_input_box_set_buffer(
        &MMI_inline_singleline_inputbox,
        MMI_singleline_inputbox_x,
        MMI_singleline_inputbox_y,
        MMI_singleline_inputbox_width,
        MMI_singleline_inputbox_height,
        buffer,
        buffer_length * 2,
        (text_length + 1) * 2,
        edit_position);
#endif /* __UCS2_ENCODING */ 
#ifdef __ASCII
    gui_create_single_line_input_box_set_buffer(
        &MMI_inline_singleline_inputbox,
        MMI_singleline_inputbox_x,
        MMI_singleline_inputbox_y,
        MMI_singleline_inputbox_width,
        MMI_singleline_inputbox_height,
        buffer,
        buffer_length,
        text_length + 1,
        edit_position);
#endif /* __ASCII */ 
    current_single_line_input_box_theme = t;
#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_INLINE_SINGLELINE_INPUTBOX_FRAME_SKIPPING))
    wgui_inline_singleline_inputbox_frame_counter = 0;
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  show_inline_singleline_inputbox
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_inline_singleline_inputbox(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_single_line_input_box_theme *t = current_single_line_input_box_theme;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    current_single_line_input_box_theme = &wgui_inline_singleline_inputbox_focussed_theme;
    gui_set_single_line_input_box_current_theme(&MMI_inline_singleline_inputbox);
     
    MMI_inline_singleline_inputbox.text_font = &MMI_medium_font;
     
    gui_show_single_line_input_box(&MMI_inline_singleline_inputbox);
    gdi_layer_blt_previous(
        MMI_inline_singleline_inputbox.x,
        MMI_inline_singleline_inputbox.y,
        MMI_inline_singleline_inputbox.x + MMI_inline_singleline_inputbox.width,
        MMI_inline_singleline_inputbox.y + MMI_inline_singleline_inputbox.height);
    current_single_line_input_box_theme = t;

}

#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_INLINE_SINGLELINE_INPUTBOX_FRAME_SKIPPING))


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_singleline_inputbox_end_frame
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_singleline_inputbox_end_frame(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_inline_singleline_inputbox_frame_counter >= 1)
    {
        gui_show_single_line_input_box(&MMI_inline_singleline_inputbox);
        gdi_layer_blt_previous(
            MMI_inline_singleline_inputbox.x,
            MMI_inline_singleline_inputbox.y,
            MMI_inline_singleline_inputbox.x + MMI_inline_singleline_inputbox.width,
            MMI_inline_singleline_inputbox.y + MMI_inline_singleline_inputbox.height);
        wgui_inline_singleline_inputbox_frame_counter = 0;
        gui_start_timer(UI_FRAME_SKIP_TIMEOUT, wgui_inline_singleline_inputbox_end_frame);
    }
}

#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_INLINE_SINGLELINE_INPUTBOX_FRAME_SKIPPING)) */ 


/*****************************************************************************
 * FUNCTION
 *  redraw_inline_singleline_inputbox
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_inline_singleline_inputbox(void)
{
#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_INLINE_SINGLELINE_INPUTBOX_FRAME_SKIPPING))
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_POPUP_REACH_MAX_LENGTH_MSG__
    if (GetExitScrnID() != POPUP_SCREENID)
    {
#endif /* __MMI_POPUP_REACH_MAX_LENGTH_MSG__ */ 
        if (wgui_inline_singleline_inputbox_frame_counter > 0)
        {
            wgui_inline_singleline_inputbox_frame_counter++;
        }
        else
        {
            wgui_inline_singleline_inputbox_frame_counter = 1;
            gui_start_timer(UI_FRAME_START_TIMEOUT, wgui_inline_singleline_inputbox_end_frame);
        }
    #ifdef __MMI_POPUP_REACH_MAX_LENGTH_MSG__
    }
    #endif 
#else /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_INLINE_SINGLELINE_INPUTBOX_FRAME_SKIPPING)) */ 

#ifdef __MMI_POPUP_REACH_MAX_LENGTH_MSG__
    if (GetExitScrnID() != POPUP_SCREENID)
    {
#endif /* __MMI_POPUP_REACH_MAX_LENGTH_MSG__ */ 
        gui_show_single_line_input_box(&MMI_inline_singleline_inputbox);
    #ifdef __MMI_POPUP_REACH_MAX_LENGTH_MSG__
    }
    #endif 
    gdi_layer_blt_previous(
        MMI_inline_singleline_inputbox.x,
        MMI_inline_singleline_inputbox.y,
        MMI_inline_singleline_inputbox.x + MMI_inline_singleline_inputbox.width,
        MMI_inline_singleline_inputbox.y + MMI_inline_singleline_inputbox.height);

#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_INLINE_SINGLELINE_INPUTBOX_FRAME_SKIPPING)) */ 
}


/*****************************************************************************
 * FUNCTION
 *  clear_inline_singleline_inputbox_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_inline_singleline_inputbox_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	extern void ClearInputEventHandler(U16 device);
    ClearInputEventHandler(MMI_DEVICE_KEY);
    clear_keyboard_key_handler();
    clear_multitap_key_handlers();
    clear_MMI_key_input_handler();
    clear_key_down_handler();
    clear_keyboard_input_handler();
    clear_inline_item_keys = UI_dummy_function;
}

void inline_edit_singleline_inputbox_multitap_input_no_draw(UI_character_type c);
void inline_edit_singleline_inputbox_multitap_input_complete_no_draw(void);
void inline_text_edit_handle_multitap_input_complete(S32 type);


/*****************************************************************************
 * FUNCTION
 *  reset_inline_singleline_inputbox
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_inline_singleline_inputbox(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_text_edit_handle_multitap_input_complete(WGUI_INLINE_TEXT_EDIT_MULTITAP_ANY);
    set_multitap_functions(
        inline_edit_singleline_inputbox_multitap_input_no_draw,
        inline_edit_singleline_inputbox_multitap_input_complete_no_draw);
#if(UI_BLINKING_CURSOR_SUPPORT)
    //gui_cancel_timer(UI_inputbox_blink_cursor);
     
    StopMyTimer(BLINKING_CURSOR);
    /* CSD end */
#endif /* (UI_BLINKING_CURSOR_SUPPORT) */ 
#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_INLINE_SINGLELINE_INPUTBOX_FRAME_SKIPPING))
    gui_cancel_timer(wgui_inline_singleline_inputbox_end_frame);
    wgui_inline_singleline_inputbox_frame_counter = 0;
#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_INLINE_SINGLELINE_INPUTBOX_FRAME_SKIPPING)) */ 
    reset_multitaps();
    clear_inline_singleline_inputbox_keys();
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_delete_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_delete_character(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_single_line_input_box_delete_character(&MMI_inline_singleline_inputbox);
    wgui_inline_list_menu_changed = 1;
    redraw_inline_singleline_inputbox();
    inline_singleline_inputbox_input_callback();
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_previous_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_previous_character(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_single_line_input_box_previous(&MMI_inline_singleline_inputbox);
    redraw_inline_singleline_inputbox();
    inline_singleline_inputbox_navigate_callback();
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_next_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_next_character(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_single_line_input_box_next(&MMI_inline_singleline_inputbox);
    redraw_inline_singleline_inputbox();
    inline_singleline_inputbox_navigate_callback();
}


/*****************************************************************************
 * FUNCTION
 *  set_inline_edit_singleline_inputbox_mask
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void set_inline_edit_singleline_inputbox_mask(U8 m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (m)
    {
        case 0:
            MMI_inline_singleline_inputbox.flags &= ~UI_SINGLE_LINE_INPUT_BOX_MASK_CHARACTERS;
            break;
        case 1:
            MMI_inline_singleline_inputbox.flags |= UI_SINGLE_LINE_INPUT_BOX_MASK_CHARACTERS;
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_resize_singleline_inputbox
 * DESCRIPTION
 *  
 * PARAMETERS
 *  width       [IN]        
 *  height      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_resize_singleline_inputbox(S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_resize_single_line_input_box(&MMI_inline_singleline_inputbox, width, height);
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_move_singleline_inputbox
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x       [IN]        
 *  y       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_move_singleline_inputbox(S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_move_single_line_input_box(&MMI_inline_singleline_inputbox, x, y);
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_register_singleline_inputbox_input_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  f       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_register_singleline_inputbox_input_callback(void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_singleline_inputbox_input_callback = f;
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_register_singleline_inputbox_navigate_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  f       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_register_singleline_inputbox_navigate_callback(void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_singleline_inputbox_navigate_callback = f;
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_toggle_insert_mode
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_toggle_insert_mode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_single_line_input_box_toggle_insert_mode(&MMI_inline_singleline_inputbox);
    redraw_inline_singleline_inputbox();
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_delete_all_characters
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_delete_all_characters(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_single_line_input_box_delete_all(&MMI_inline_singleline_inputbox);
    wgui_inline_list_menu_changed = 1;
    redraw_inline_singleline_inputbox();
    inline_singleline_inputbox_input_callback();
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_delete_current_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_delete_current_character(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_single_line_input_box_delete_current_character(&MMI_inline_singleline_inputbox);
    wgui_inline_list_menu_changed = 1;
    redraw_inline_singleline_inputbox();
    inline_singleline_inputbox_input_callback();
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_multitap_input
 * DESCRIPTION
 *  
 * PARAMETERS
 *  c       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_multitap_input(UI_character_type c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    single_line_input_box *b = &MMI_inline_singleline_inputbox;
    U32 flag = 0;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if( (c == '+') && (b->text[0] != '+') && b->current_text_p == b->text && ((b->last_position_p - b->text) >= (b->available_length)) )
    {
        b->available_length =
        (wgui_inline_singleline_inputbox_buffer_size + 1) * ENCODING_LENGTH;
        flag = 1;
    }    
    gui_single_line_input_box_insert_multitap_character(&MMI_inline_singleline_inputbox, c);
    if(flag)
    {
        switch (wgui_inline_text_edit_active_multitap)
        {
            case WGUI_INLINE_TEXT_EDIT_MULTITAP_STAR:
                if (MMI_inline_singleline_inputbox.flags & UI_SINGLE_LINE_INPUT_BOX_STATE_MULTITAP)
                {
                    gui_cancel_timer(inline_edit_phone_number_input_complete_multitap_star_key);
                    gui_multitap_input_complete(&wgui_inline_text_edit_multitap_star_key);
                }
                break;
            case WGUI_INLINE_TEXT_EDIT_MULTITAP_ZERO:
 
                if (MMI_inline_singleline_inputbox.flags & UI_SINGLE_LINE_INPUT_BOX_STATE_MULTITAP)
                {
                    gui_cancel_timer(inline_edit_phone_number_input_complete_multitap_zero_key);
                    gui_multitap_input_complete(&wgui_inline_text_edit_multitap_zero_key);
                }
                break;
        }
    }

    
    wgui_inline_list_menu_changed = 1;
    redraw_inline_singleline_inputbox();
    inline_singleline_inputbox_input_callback();
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_multitap_input_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_multitap_input_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_single_line_input_box_confirm_multitap_character(&MMI_inline_singleline_inputbox);
    wgui_inline_list_menu_changed = 1;
    redraw_inline_singleline_inputbox();
    inline_singleline_inputbox_input_callback();
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_multitap_input_no_draw
 * DESCRIPTION
 *  
 * PARAMETERS
 *  c       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_multitap_input_no_draw(UI_character_type c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_single_line_input_box_insert_multitap_character(&MMI_inline_singleline_inputbox, c);
    wgui_inline_list_menu_changed = 1;
    inline_singleline_inputbox_input_callback();
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_multitap_input_complete_no_draw
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_multitap_input_complete_no_draw(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_single_line_input_box_confirm_multitap_character(&MMI_inline_singleline_inputbox);
    wgui_inline_list_menu_changed = 1;
    inline_singleline_inputbox_input_callback();
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_direct_input
 * DESCRIPTION
 *  
 * PARAMETERS
 *  c       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_direct_input(UI_character_type c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    single_line_input_box *b = &MMI_inline_singleline_inputbox;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if( (c == '+') && (b->text[0] != '+') && b->current_text_p == b->text && ((b->last_position_p - b->text) >= (b->available_length)) )
    {
        b->available_length =
        (wgui_inline_singleline_inputbox_buffer_size + 1) * ENCODING_LENGTH;
    }  
    gui_single_line_input_box_insert_character(&MMI_inline_singleline_inputbox, c);
    wgui_inline_list_menu_changed = 1;
    redraw_inline_singleline_inputbox();
    inline_singleline_inputbox_input_callback();
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  keyc        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler(S32 keyc)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (keyc > 0x1f)
    {
        inline_edit_singleline_inputbox_direct_input((U8) keyc);
    }
    else if (keyc == 0x08)
    {
        inline_edit_singleline_inputbox_delete_character();
    }
    else if (keyc == 0x1b)
    {
        inline_edit_singleline_inputbox_delete_all_characters();
    }
    else if (keyc == 0x0d);
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_numeric_keyboard_input_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  keyc        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_numeric_keyboard_input_handler(S32 keyc)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (keyc >= '0' && keyc <= '9')
    {
        inline_edit_singleline_inputbox_direct_input((U8) keyc);
    }
    else if (keyc == 0x08)
    {
        inline_edit_singleline_inputbox_delete_character();
    }
    else if (keyc == 0x1b)
    {
        inline_edit_singleline_inputbox_delete_all_characters();
    }
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_singleline_inputbox_handle_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  k       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_singleline_inputbox_handle_key_down(MMI_key_code_type k)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_list_menu_changed = 1;
    inline_edit_singleline_inputbox_direct_input((UI_character_type) ('0' + k));
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_change_singleline_inputbox_mode
 * DESCRIPTION
 *  
 * PARAMETERS
 *  mode        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_FULL_KEYPAD__
extern  void  setCharSet(U8 charset);
extern void register_inline_direct_input_function(void);
#endif

void inline_edit_change_singleline_inputbox_mode(U8 mode)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (mode)
    {
        case INPUT_MODE_MULTITAP_UPPERCASE_ABC:
            #ifndef __MMI_FULL_KEYPAD__
	            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_ABC);
	            register_multitap_no_draw_key_handlers();
	            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
	            set_multitap_functions(
	                inline_edit_singleline_inputbox_multitap_input,
	                inline_edit_singleline_inputbox_multitap_input_complete);
            #else
	            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_ABC);
	            setCharSet(INPUT_MODE_MULTITAP_UPPERCASE_ABC);
	            register_inline_direct_input_function();
            #endif
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_ABC:
            #ifndef __MMI_FULL_KEYPAD__
	            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_ABC);
	            register_multitap_no_draw_key_handlers();
	            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
	            set_multitap_functions(
	                inline_edit_singleline_inputbox_multitap_input,
	                inline_edit_singleline_inputbox_multitap_input_complete);
            #else
	            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_ABC);
	            setCharSet(INPUT_MODE_MULTITAP_LOWERCASE_ABC);
	            register_inline_direct_input_function();
            #endif
            break;
        case INPUT_MODE_123:
            change_multitap_mode(INPUT_MODE_123);
            clear_multitap_key_handlers();
            register_MMI_key_input_handler();
            register_key_down_handler(inline_edit_singleline_inputbox_handle_key_down);
            register_keyboard_input_handler(inline_edit_singleline_inputbox_numeric_keyboard_input_handler);
            break;

             
    #if defined (__MMI_MULTITAP_THAI__)
        case INPUT_MODE_MULTITAP_THAI:
            change_multitap_mode(INPUT_MODE_MULTITAP_THAI);
             
            //change_inputbox_type_multitapthai(SINGLELINE_INPUTBOX);
            register_multitap_no_draw_key_handlers();
            //register_MMI_key_input_extend_handler();    /* msz065_20050418 */
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined (__MMI_MULTITAP_THAI__) */ 

             
    #if defined(__MMI_MULTITAP_SPANISH__)
        case INPUT_MODE_MULTITAP_UPPERCASE_SPANISH:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_SPANISH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_SPANISH:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_SPANISH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_SPANISH__) */ 
//qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
            case	INPUT_MODE_MMI_MULTITAP_TAMIL:
            change_multitap_mode(INPUT_MODE_MMI_MULTITAP_TAMIL);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
            inline_edit_singleline_inputbox_multitap_input,
            inline_edit_singleline_inputbox_multitap_input_complete);
            break;
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
            case	INPUT_MODE_MMI_MULTITAP_BENGALI:
            change_multitap_mode(INPUT_MODE_MMI_MULTITAP_BENGALI);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
            inline_edit_singleline_inputbox_multitap_input,
            inline_edit_singleline_inputbox_multitap_input_complete);
            break;
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
            case	INPUT_MODE_MMI_MULTITAP_PUNJABI:
            change_multitap_mode(INPUT_MODE_MMI_MULTITAP_PUNJABI);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
            inline_edit_singleline_inputbox_multitap_input,
            inline_edit_singleline_inputbox_multitap_input_complete);
            break;
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
            case	INPUT_MODE_MMI_MULTITAP_TELUGU:
            change_multitap_mode(INPUT_MODE_MMI_MULTITAP_TELUGU);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
            inline_edit_singleline_inputbox_multitap_input,
            inline_edit_singleline_inputbox_multitap_input_complete);
            break;
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
            case	INPUT_MODE_MMI_MULTITAP_KANNADA:
            change_multitap_mode(INPUT_MODE_MMI_MULTITAP_KANNADA);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
            inline_edit_singleline_inputbox_multitap_input,
            inline_edit_singleline_inputbox_multitap_input_complete);
            break;
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
            case	INPUT_MODE_MMI_MULTITAP_MALAYALAM:
            change_multitap_mode(INPUT_MODE_MMI_MULTITAP_MALAYALAM);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
            inline_edit_singleline_inputbox_multitap_input,
            inline_edit_singleline_inputbox_multitap_input_complete);
            break;
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
            case	INPUT_MODE_MMI_MULTITAP_ORIYA:
            change_multitap_mode(INPUT_MODE_MMI_MULTITAP_ORIYA);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
            inline_edit_singleline_inputbox_multitap_input,
            inline_edit_singleline_inputbox_multitap_input_complete);
            break;
#endif
//qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
            case	INPUT_MODE_MMI_MULTITAP_MARATHI:
            change_multitap_mode(INPUT_MODE_MMI_MULTITAP_MARATHI);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
            inline_edit_singleline_inputbox_multitap_input,
            inline_edit_singleline_inputbox_multitap_input_complete);
            break;
#endif

#if defined(__MMI_MULTITAP_URDU__)
            case	INPUT_MODE_MMI_MULTITAP_URDU:
            change_multitap_mode(INPUT_MODE_MMI_MULTITAP_URDU);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
            inline_edit_singleline_inputbox_multitap_input,
            inline_edit_singleline_inputbox_multitap_input_complete);
            break;
#endif
             
    #if defined(__MMI_MULTITAP_DANISH__)
        case INPUT_MODE_MULTITAP_UPPERCASE_DANISH:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_DANISH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_DANISH:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_DANISH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_DANISH__) */ 
             
             
    #if defined(__MMI_MULTITAP_POLISH__)
        case INPUT_MODE_MULTITAP_UPPERCASE_POLISH:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_POLISH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_POLISH:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_POLISH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_POLISH__) */ 
             
    #if defined(__MMI_MULTITAP_FRENCH__)
        case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_FRENCH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_FRENCH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_FRENCH__) */ 
    #if defined(__MMI_MULTITAP_MYANMAR__)
        case INPUT_MODE_MULTITAP_MYANMAR:
            change_multitap_mode(INPUT_MODE_MULTITAP_MYANMAR);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
            inline_edit_singleline_inputbox_multitap_input,
            inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif
    #if defined(__MMI_MULTITAP_GERMAN__)
        case INPUT_MODE_MULTITAP_UPPERCASE_GERMAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_GERMAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_GERMAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_GERMAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_GERMAN__) */ 
    #if defined(__MMI_MULTITAP_ITALIAN__)
        case INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_ITALIAN__) */ 
    #if defined(__MMI_MULTITAP_RUSSIAN__)
        case INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_RUSSIAN__) */ 
 
    #if defined(__MMI_MULTITAP_BULGARIAN__)
        case INPUT_MODE_MULTITAP_UPPERCASE_BULGARIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_BULGARIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_BULGARIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_BULGARIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_BULGARIAN__) */ 
 
    #if defined(__MMI_MULTITAP_TURKISH__)
        case INPUT_MODE_MULTITAP_UPPERCASE_TURKISH:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_TURKISH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_TURKISH:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_TURKISH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_TURKISH__) */ 
    #if defined(__MMI_MULTITAP_PORTUGUESE__)
        case INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_PORTUGUESE__) */ 
    #if defined(__MMI_MULTITAP_INDONESIAN__)
        case INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_INDONESIAN__) */ 
             
    #if defined(__MMI_MULTITAP_CZECH__)
        case INPUT_MODE_MULTITAP_UPPERCASE_CZECH:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_CZECH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_CZECH:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_CZECH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_CZECH__) */ 
             
    #if defined(__MMI_MULTITAP_MALAY__)
        case INPUT_MODE_MULTITAP_UPPERCASE_MALAY:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_MALAY);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_MALAY:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_MALAY);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_MALAY__) */ 
    #if defined(__MMI_MULTITAP_VIETNAMESE__)
        case INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_VIETNAMESE__) */ 
             
    #if defined(__MMI_MULTITAP_FINNISH__)
        case INPUT_MODE_MULTITAP_UPPERCASE_FINNISH:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_FINNISH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_FINNISH:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_FINNISH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_FINNISH__) */ 
    #if defined(__MMI_MULTITAP_HUNGARIAN__)
        case INPUT_MODE_MULTITAP_UPPERCASE_HUNGARIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_HUNGARIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_HUNGARIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_HUNGARIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_HUNGARIAN__) */ 
             
            /* CSD end */
             
    #if defined(__MMI_MULTITAP_HEBREW__)
        case INPUT_MODE_MULTITAP_HEBREW:
            change_multitap_mode(INPUT_MODE_MULTITAP_HEBREW);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_HEBREW__) */ 
             
 
    #if defined(__MMI_MULTITAP_SLOVAK__)
        case INPUT_MODE_MULTITAP_UPPERCASE_SLOVAK:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_SLOVAK);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_SLOVAK:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_SLOVAK);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_SLOVAK__) */ 
    #if defined(__MMI_MULTITAP_DUTCH__)
        case INPUT_MODE_MULTITAP_UPPERCASE_DUTCH:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_DUTCH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_DUTCH:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_DUTCH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_DUTCH__) */ 
    #if defined(__MMI_MULTITAP_ARABIC__)
        case INPUT_MODE_MULTITAP_ARABIC:
            change_multitap_mode(INPUT_MODE_MULTITAP_ARABIC);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_ARABIC__) */ 
            /* Norwegian start */
    #if defined(__MMI_MULTITAP_NORWEGIAN__)
        case INPUT_MODE_MULTITAP_UPPERCASE_NORWEGIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_NORWEGIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_NORWEGIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_NORWEGIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_NORWEGIAN__) */ 
            //Norwegian end
 
             
    #if defined(__MMI_MULTITAP_SWEDISH__)
        case INPUT_MODE_MULTITAP_UPPERCASE_SWEDISH:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_SWEDISH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_SWEDISH:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_SWEDISH);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_SWEDISH__) */ 
             
             
    #if defined(__MMI_MULTITAP_CROATIAN__)
        case INPUT_MODE_MULTITAP_UPPERCASE_CROATIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_CROATIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_CROATIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_CROATIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_CROATIAN__) */ 
    #if defined(__MMI_MULTITAP_ROMANIAN__)
        case INPUT_MODE_MULTITAP_UPPERCASE_ROMANIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_ROMANIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_ROMANIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_ROMANIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_ROMANIAN__) */ 
    #if defined(__MMI_MULTITAP_SLOVENIAN__)
        case INPUT_MODE_MULTITAP_UPPERCASE_SLOVENIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_SLOVENIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_SLOVENIAN:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_SLOVENIAN);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_SLOVENIAN__) */ 
             
             
    #if defined(__MMI_MULTITAP_GREEK__)
        case INPUT_MODE_MULTITAP_UPPERCASE_GREEK:
            change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_GREEK);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
        case INPUT_MODE_MULTITAP_LOWERCASE_GREEK:
            change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_GREEK);
            register_multitap_no_draw_key_handlers();
            register_keyboard_input_handler(inline_edit_singleline_inputbox_alphabetic_keyboard_input_handler);
            set_multitap_functions(
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            break;
    #endif /* defined(__MMI_MULTITAP_GREEK__) */ 
             
    }
}

void clear_inline_singleline_inputbox_keys(void);
void handle_inline_text_edit_complete(void);


/*****************************************************************************
 * FUNCTION
 *  inline_text_edit_handle_up_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_text_edit_handle_up_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_fixed_list_menu.n_items != 1)
    {
        handle_inline_text_edit_complete();
        inline_fixed_list_goto_previous_item();
    }
}


/*****************************************************************************
 * FUNCTION
 *  inline_text_edit_handle_down_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_text_edit_handle_down_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_fixed_list_menu.n_items != 1)
    {
        handle_inline_text_edit_complete();
        inline_fixed_list_goto_next_item();
    }
}


/*****************************************************************************
 * FUNCTION
 *  inline_singleline_inputbox_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  vkey_code       [IN]        
 *  key_state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_singleline_inputbox_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 37:
                inline_edit_singleline_inputbox_previous_character();
                break;
            case 38:
                inline_text_edit_handle_up_arrow();
                break;
            case 39:
                inline_edit_singleline_inputbox_next_character();
                break;
            case 40:
                inline_text_edit_handle_down_arrow();
                break;
            case 45:
                inline_edit_singleline_inputbox_toggle_insert_mode();
                break;
            case 46:
                inline_edit_singleline_inputbox_delete_current_character();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}


/*****************************************************************************
 * FUNCTION
 *  register_inline_singleline_inputbox_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_inline_singleline_inputbox_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(inline_edit_singleline_inputbox_previous_character, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_edit_singleline_inputbox_next_character, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    /* SetKeyHandler(inline_edit_singleline_inputbox_delete_character,KEY_CLEAR,KEY_EVENT_DOWN); */
    register_keyboard_key_handler(inline_singleline_inputbox_key_handler);
}


/*****************************************************************************
 * FUNCTION
 *  inline_singleline_inputbox_test_change_input_mode
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
U8 inline_singleline_inputbox_test_change_input_mode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_inline_singleline_inputbox.flags & UI_SINGLE_LINE_INPUT_BOX_USE_ENCODING_BASED_LENGTH)
    {
        if (MMI_inline_singleline_inputbox.UCS2_count <= 0)
        {
            if ((MMI_inline_singleline_inputbox.flags & UI_SINGLE_LINE_INPUT_BOX_ONE_LESS_CHARACTER) &&
                (MMI_inline_singleline_inputbox.text_length >=
                 ((MMI_inline_singleline_inputbox.available_length >> 1) - 1)))
            {
                return (0);
            }
            else if (MMI_inline_singleline_inputbox.text_length >=
                     (MMI_inline_singleline_inputbox.available_length >> 1))
            {
                return (0);
            }
            else
            {
                return (1);
            }
        }
        else
        {
            return (1);
        }
    }
    else
    {
        return (1);
    }
}

void wgui_hide_inline_input_method(void);
void wgui_display_inline_input_method(void);


/*****************************************************************************
 * FUNCTION
 *  handle_inline_text_edit_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_text_edit_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 input_type = current_wgui_inline_text_edit_item->input_type & INPUT_TYPE_MASK;


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_inline_singleline_inputbox();
    current_wgui_inline_text_edit_item->UCS2_count = MMI_inline_singleline_inputbox.UCS2_count;
    register_inline_fixed_list_keys();
    gui_strcpy(
        (UI_string_type) current_wgui_inline_text_edit_item->buffer,
        (UI_string_type) wgui_inline_text_edit_buffer);
    if ((input_type == INPUT_TYPE_NUMERIC_PASSWORD) || (input_type == INPUT_TYPE_ALPHANUMERIC_PASSWORD) ||
        (input_type == INPUT_TYPE_SAT_NUMERIC_PASSWORD))
    {
        inline_edit_prepare_mask_buffer(
            current_wgui_inline_text_edit_item->buffer,
            current_wgui_inline_text_edit_item->mask_buffer);
        *current_wgui_inline_text_edit_text_p = current_wgui_inline_text_edit_item->mask_buffer;
    }
    else
    {
         
    #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        if ((IsInlineItemDefaultTextEnable()) &&
            (gui_strlen((UI_string_type) current_wgui_inline_text_edit_item->buffer) == 0))
        {
            *current_wgui_inline_text_edit_text_p = current_wgui_inline_text_edit_item->default_item_text;
        }
        else
    #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
             

            *current_wgui_inline_text_edit_text_p = current_wgui_inline_text_edit_item->buffer;
         
    #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        MMI_fixed_icontext_menuitems[wgui_inline_item_highlighted_index].item_text =
            (UI_string_type) (*current_wgui_inline_text_edit_text_p);
    #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
         
    }
    inline_text_edit_RSK_function = NULL;
    clear_left_softkey();
    clear_right_softkey();
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_close_input_box();
#endif 
    wgui_inline_item_handled = 0;
    current_wgui_inline_text_edit_item->edit_complete_callback(
                                            current_wgui_inline_text_edit_item->buffer,
                                            current_wgui_inline_text_edit_item->buffer_size);
    redraw_current_inline_item = UI_dummy_function;
    wgui_inputbox_UCS2_count_function = NULL;
    wgui_inputbox_not_empty_callback = NULL;
    wgui_inputbox_empty_callback = NULL;
    wgui_inputbox_validation_callback = NULL;
    complete_inline_item_edit = UI_dummy_function;
    test_change_input_mode = NULL;
    MMI_allow_only_english_input_modes = 0;
    /*
 
     * * to check which inline data struct(inline text edit or full screen edit) history
     * * will be stored in history, we have to reset it while switching to other inline item.
     * * Otherwise, if other inline item(INLINE_ITEM_TYPE_FULL_SCREEN_EDIT) is not set wgui_current_inline_item_type, then
     * * wgui_current_inline_item_type keeps the old setting and causes not required input box history be stored in history
     * * One example:
     * * 1. In PHB email inline editing box wgui_current_inline_item_type= INLINE_ITEM_TYPE_TEXT_EDIT   and input_type is abc
     * * 2. goto previous one item(full screen edit while pressing LSK"Edit")
     * * 3. At this momnet, charger in and cuase the INLINE_ITEM_TYPE_TEXT_EDIT(input method is abc) stored in history
     * * 4. goto previous item(home number editing box)
     * * 5. the history is re-stored into current inline text editing box
     * * 6. the input method is abc, but 123
     */
     
    //wgui_current_inline_item_type = 0;
    //CSD end

    //gui_cancel_timer(UI_inputbox_blink_cursor);
     
    StopMyTimer(BLINKING_CURSOR);
    /* CSD end */

    gui_cancel_timer(wgui_hide_inline_input_method);
    gui_cancel_timer(wgui_display_inline_input_method);
     
    gui_cancel_timer(inline_edit_phone_number_input_complete_multitap_star_key);
    gui_cancel_timer(inline_edit_phone_number_input_complete_multitap_zero_key);
    /* CSD end */
     
    set_current_input_box_type(EDITOR_NONE_INPUT_BOX);
#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
     
    //But in in-line editor, redrawing is needed when completing in-line item
#ifdef __MMI_WGUI_CSK_ENABLE__
    redraw_softkey(MMI_CENTER_SOFTKEY);
#endif 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 

     
    // draw_title();
     
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_text_edit_cancel
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_text_edit_cancel(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    reset_inline_singleline_inputbox();
    register_inline_fixed_list_keys();
    inline_text_edit_RSK_function = NULL;
    clear_left_softkey();
    clear_right_softkey();
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_close_input_box();
#endif 
    wgui_inline_item_handled = 0;
    wgui_text_menuitem_restart_scrolling();
    show_fixed_list();
    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    wgui_inputbox_UCS2_count_function = NULL;
    wgui_inputbox_not_empty_callback = NULL;
    wgui_inputbox_empty_callback = NULL;
    wgui_inputbox_validation_callback = NULL;
    complete_inline_item_edit = UI_dummy_function;
    test_change_input_mode = NULL;
    MMI_allow_only_english_input_modes = 0;

    //gui_cancel_timer(UI_inputbox_blink_cursor);
     
    StopMyTimer(BLINKING_CURSOR);
    /* CSD end */

    gui_cancel_timer(wgui_hide_inline_input_method);
    gui_cancel_timer(wgui_display_inline_input_method);
     
    set_current_input_box_type(EDITOR_NONE_INPUT_BOX);
#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
     
    //But in in-line editor, redrawing is needed when completing in-line item
#ifdef __MMI_WGUI_CSK_ENABLE__
    redraw_softkey(MMI_CENTER_SOFTKEY);
#endif 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 

    gdi_layer_unlock_frame_buffer();
  //  gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

     
    // draw_title();
     
}

/* Clear / Back RSK functionality handling for inline text edit   */


/*****************************************************************************
 * FUNCTION
 *  handle_inline_text_edit_right_softkey_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_text_edit_right_softkey_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_inline_singleline_inputbox_decimal_input_mode_flag)
    {
        if (wgui_inline_singleline_inputbox_decimal_flag)
        {
            UI_buffer_type p = MMI_inline_singleline_inputbox.current_text_p;
            UI_character_type c;

            if (p != MMI_inline_singleline_inputbox.text)
            {
                UI_STRING_GET_PREVIOUS_CHARACTER(p, c);
                if (c == (UI_character_type) '.')
                {
                    wgui_inline_singleline_inputbox_decimal_flag = 0;
                }
            }
        }
        if (gui_single_line_input_box_get_text_length(&MMI_inline_singleline_inputbox) > 0)
        {
            inline_edit_singleline_inputbox_delete_character();
        }
    }
    else
    {
        if (gui_single_line_input_box_get_text_length(&MMI_inline_singleline_inputbox) > 0)
        {
            inline_edit_singleline_inputbox_delete_character();
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_text_edit_right_softkey_up
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_text_edit_right_softkey_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (inline_text_edit_RSK_function != NULL)
    {
        inline_text_edit_RSK_function();
    }
}


/*****************************************************************************
 * FUNCTION
 *  register_inline_text_edit_handle_right_softkey_up
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_inline_text_edit_handle_right_softkey_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_right_softkey_function(handle_inline_text_edit_right_softkey_up, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  register_inline_text_edit_handle_clear_key_up
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_inline_text_edit_handle_clear_key_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(handle_inline_text_edit_right_softkey_up, KEY_CLEAR, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_text_edit_right_softkey_long_press
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_text_edit_right_softkey_long_press(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_edit_singleline_inputbox_delete_all_characters();
    if (wgui_inline_singleline_inputbox_decimal_input_mode_flag)
    {
        wgui_inline_singleline_inputbox_decimal_flag = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_text_edit_input
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_text_edit_input(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gui_single_line_input_box_get_text_length(&MMI_inline_singleline_inputbox) > 0)
    {
    #ifdef __MMI_WITH_C_KEY__
        /* CSD for RSK display with C Key */
        /* Need to consider wgui_inline_list_menu_disable_done */
        if (wgui_inline_list_menu_changed && !(wgui_inline_list_menu_disable_done))
        {
            set_right_softkey_label(NULL);
            set_right_softkey_icon(NULL);
             
        #ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
            /* To show proper key arrangement */
            show_softkey_background();
            redraw_left_softkey();
        #endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
             
            redraw_right_softkey();
        }
        if (inline_text_edit_RSK_label_clear == 0)
        {
            //reset_right_softkey();
            //set_right_softkey_label(NULL);
            //set_right_softkey_icon(NULL);
            //redraw_right_softkey();
            if (wgui_inputbox_not_empty_callback != NULL)
            {
                wgui_inputbox_not_empty_callback();
            }
            inline_text_edit_RSK_label_clear = 1;
            SetKeyHandler(handle_inline_text_edit_right_softkey_down, KEY_CLEAR, KEY_EVENT_DOWN);
            SetKeyHandler(UI_dummy_function, KEY_CLEAR, KEY_EVENT_UP);
            SetKeyHandler(handle_inline_text_edit_right_softkey_long_press, KEY_CLEAR, KEY_EVENT_LONG_PRESS);
        }
    #else /* __MMI_WITH_C_KEY__ */ 
        if (inline_text_edit_RSK_label_clear == 0)
        {
            set_right_softkey_label((UI_string_type) get_string(WGUI_CATEGORY_CLEAR_STRING_ID));
            set_right_softkey_icon(NULL);
            if (wgui_inputbox_not_empty_callback != NULL)
            {
                wgui_inputbox_not_empty_callback();
            }
            register_right_softkey_handler();
            redraw_right_softkey();
            inline_text_edit_RSK_label_clear = 1;
            set_right_softkey_function(UI_dummy_function, KEY_EVENT_UP);
            set_right_softkey_function(handle_inline_text_edit_right_softkey_down, KEY_EVENT_DOWN);

#ifdef __MMI_FULL_KEYPAD__
            SetKeyHandler(handle_inline_text_edit_right_softkey_down, KEY_BACKSPACE1, KEY_EVENT_DOWN);
            SetKeyHandler(UI_dummy_function, KEY_BACKSPACE1, KEY_EVENT_UP);
            SetKeyHandler(handle_inline_text_edit_right_softkey_long_press, KEY_BACKSPACE1, KEY_EVENT_LONG_PRESS);
#endif
			
        }
    #endif /* __MMI_WITH_C_KEY__ */ 
    }
    else
    {
        if (inline_text_edit_RSK_label_clear == 1)
        {
            if (wgui_inline_list_menu_changed)
            {
                wgui_inline_text_edit_set_RSK_BACK_function();
                inline_text_edit_RSK_label_clear = 0;
                if (wgui_inputbox_empty_callback != NULL)
                {
                    wgui_inputbox_empty_callback();
                }
            }
            else
            {
            #if defined(__MMI_WITH_C_KEY__)
                 
                if (wgui_inputbox_empty_callback != NULL)
                {
                    wgui_inputbox_empty_callback();
                }
                inline_text_edit_RSK_label_clear = 0;
                SetKeyHandler(UI_dummy_function, KEY_CLEAR, KEY_EVENT_DOWN);
                SetKeyHandler(register_inline_text_edit_handle_clear_key_up, KEY_CLEAR, KEY_EVENT_UP);
                SetKeyHandler(UI_dummy_function, KEY_CLEAR, KEY_EVENT_LONG_PRESS);
            #else /* defined(__MMI_WITH_C_KEY__) */ 
                set_right_softkey_label(inline_text_edit_RSK_label_string);
                set_right_softkey_icon(inline_text_edit_RSK_label_icon);
                if (wgui_inputbox_empty_callback != NULL)
                {
                    wgui_inputbox_empty_callback();
                }
                register_right_softkey_handler();
                redraw_right_softkey();
                inline_text_edit_RSK_label_clear = 0;
                set_right_softkey_function(register_inline_text_edit_handle_right_softkey_up, KEY_EVENT_UP);
                set_right_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
#ifdef __MMI_FULL_KEYPAD__
            SetKeyHandler(UI_dummy_function, KEY_BACKSPACE1, KEY_EVENT_DOWN);
            SetKeyHandler(register_inline_text_edit_handle_clear_key_up, KEY_BACKSPACE1, KEY_EVENT_UP);
            SetKeyHandler(UI_dummy_function, KEY_BACKSPACE1, KEY_EVENT_LONG_PRESS);
#endif
            #endif /* defined(__MMI_WITH_C_KEY__) */ 
            }
        }
    }
    if (MMI_inline_singleline_inputbox.flags & UI_SINGLE_LINE_INPUT_BOX_PLUS_CHARACTER_HANDLING)
    {
        if (MMI_inline_singleline_inputbox.text[0] == '+')
        {
            MMI_inline_singleline_inputbox.available_length =
                (wgui_inline_singleline_inputbox_buffer_size + 1) * ENCODING_LENGTH;
        }
        else
        {
            MMI_inline_singleline_inputbox.available_length =
                wgui_inline_singleline_inputbox_buffer_size * ENCODING_LENGTH;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_text_edit_navigate
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_text_edit_navigate(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gui_single_line_input_box_test_first_position(&MMI_inline_singleline_inputbox))
    {
        if (inline_text_edit_RSK_label_clear == 1)
        {
            if (wgui_inline_list_menu_changed)
            {
                wgui_inline_text_edit_set_RSK_BACK_function();
                inline_text_edit_RSK_label_clear = 0;
            }
            else
            {
                set_right_softkey_label(inline_text_edit_RSK_label_string);
                set_right_softkey_icon(inline_text_edit_RSK_label_icon);
                register_right_softkey_handler();
                redraw_right_softkey();
                inline_text_edit_RSK_label_clear = 0;
                set_right_softkey_function(handle_inline_text_edit_right_softkey_up, KEY_EVENT_UP);
                set_right_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
            }
        }
    }
    else
    {
        if (inline_text_edit_RSK_label_clear == 0)
        {
            set_right_softkey_label((UI_string_type) get_string(WGUI_CATEGORY_CLEAR_STRING_ID));
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            redraw_right_softkey();
            inline_text_edit_RSK_label_clear = 1;
            set_right_softkey_function(UI_dummy_function, KEY_EVENT_UP);
            set_right_softkey_function(handle_inline_text_edit_right_softkey_down, KEY_EVENT_DOWN);
        }
    }
}




/*****************************************************************************
 * FUNCTION
 *  inline_text_edit_handle_multitap_input_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  type        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_text_edit_handle_multitap_input_complete(S32 type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_inline_text_edit_active_multitap == WGUI_INLINE_TEXT_EDIT_MULTITAP_NONE)
    {
        return;
    }
    switch (wgui_inline_text_edit_active_multitap)
    {
        case WGUI_INLINE_TEXT_EDIT_MULTITAP_STAR:
            if (type == WGUI_INLINE_TEXT_EDIT_MULTITAP_STAR)
            {
                return;
            }
            if (MMI_inline_singleline_inputbox.flags & UI_SINGLE_LINE_INPUT_BOX_STATE_MULTITAP)
            {
                gui_cancel_timer(inline_edit_phone_number_input_complete_multitap_star_key);
                gui_multitap_input_complete(&wgui_inline_text_edit_multitap_star_key);
            }
            break;
             
        case WGUI_INLINE_TEXT_EDIT_MULTITAP_ZERO:
            if (type == WGUI_INLINE_TEXT_EDIT_MULTITAP_ZERO)
            {
                return;
            }
            if (MMI_inline_singleline_inputbox.flags & UI_SINGLE_LINE_INPUT_BOX_STATE_MULTITAP)
            {
                gui_cancel_timer(inline_edit_phone_number_input_complete_multitap_zero_key);
                gui_multitap_input_complete(&wgui_inline_text_edit_multitap_zero_key);
            }
            break;
 
    }
    wgui_inline_text_edit_active_multitap = WGUI_INLINE_TEXT_EDIT_MULTITAP_NONE;
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_phone_number_input_complete_multitap_star_key
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_phone_number_input_complete_multitap_star_key(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_multitap_input_complete(&wgui_inline_text_edit_multitap_star_key);
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_phone_number_input_handle_multitap_star_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_phone_number_input_handle_multitap_star_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_text_edit_handle_multitap_input_complete(WGUI_INLINE_TEXT_EDIT_MULTITAP_STAR);
    wgui_inline_text_edit_active_multitap = WGUI_INLINE_TEXT_EDIT_MULTITAP_STAR;
    gui_change_multitap_input_state(&wgui_inline_text_edit_multitap_star_key);
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_phone_number_input_handle_multitap_star_key_up
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_phone_number_input_handle_multitap_star_key_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_start_timer(MMI_MULTITAP_TIMEOUT, inline_edit_phone_number_input_complete_multitap_star_key);
}

 
extern void MMI_key_0_down(void);
extern void MMI_key_0_up(void);


/*****************************************************************************
 * FUNCTION
 *  inline_edit_phone_number_input_complete_multitap_zero_key
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_phone_number_input_complete_multitap_zero_key(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_multitap_input_complete(&wgui_inline_text_edit_multitap_zero_key);
    SetKeyHandler(MMI_key_0_down, KEY_0, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_0_down, KEY_0, KEY_REPEAT);
    SetKeyHandler(MMI_key_0_up, KEY_0, KEY_EVENT_UP);

}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_phone_number_input_handle_multitap_zero_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_phone_number_input_handle_multitap_zero_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_text_edit_handle_multitap_input_complete(WGUI_INLINE_TEXT_EDIT_MULTITAP_ZERO);
    wgui_inline_text_edit_active_multitap = WGUI_INLINE_TEXT_EDIT_MULTITAP_ZERO;
    gui_change_multitap_input_state(&wgui_inline_text_edit_multitap_zero_key);
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_phone_number_input_handle_multitap_zero_key_up
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_phone_number_input_handle_multitap_zero_key_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_start_timer(MMI_MULTITAP_TIMEOUT + 400, inline_edit_phone_number_input_complete_multitap_zero_key);
}


/*****************************************************************************
 * FUNCTION
 *  inline_edit_phone_number_input_handle_zero_key_long_press
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_phone_number_input_handle_zero_key_long_press(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(UI_dummy_function, KEY_0, KEY_REPEAT);
    inline_edit_singleline_inputbox_delete_character();
    inline_edit_phone_number_input_handle_multitap_zero_key_down();
    SetKeyHandler(inline_edit_phone_number_input_handle_multitap_zero_key_down, KEY_0, KEY_EVENT_DOWN);
    SetKeyHandler(inline_edit_phone_number_input_handle_multitap_zero_key_up, KEY_0, KEY_EVENT_UP);
}

/* CSD end */


/*****************************************************************************
 * FUNCTION
 *  inline_edit_phone_number_input_handle_hash_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_phone_number_input_handle_hash_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_text_edit_handle_multitap_input_complete(WGUI_INLINE_TEXT_EDIT_MULTITAP_ANY);
    inline_edit_singleline_inputbox_direct_input((UI_character_type) '#');
}

 


/*****************************************************************************
 * FUNCTION
 *  inline_edit_phone_number_input_handle_star_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_phone_number_input_handle_star_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_text_edit_handle_multitap_input_complete(WGUI_INLINE_TEXT_EDIT_MULTITAP_ANY);
    inline_edit_singleline_inputbox_direct_input((UI_character_type) '*');
}

/* CSD end */


/*****************************************************************************
 * FUNCTION
 *  inline_edit_phone_number_input_numeric_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  k       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_edit_phone_number_input_numeric_key_handler(MMI_key_code_type k)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_text_edit_handle_multitap_input_complete(WGUI_INLINE_TEXT_EDIT_MULTITAP_ANY);
    inline_edit_singleline_inputbox_direct_input((UI_character_type) ('0' + k));

     
#ifdef __MMI_MULTITAP_KEY_0__
    SetKeyHandler(MMI_key_0_down, KEY_0, KEY_EVENT_DOWN);
    SetKeyHandler(MMI_key_0_down, KEY_0, KEY_REPEAT);
    SetKeyHandler(MMI_key_0_up, KEY_0, KEY_EVENT_UP);
#endif /* __MMI_MULTITAP_KEY_0__ */ 
    /* CSD end */
}


/*****************************************************************************
 * FUNCTION
 *  inline_singleline_inputbox_keypad_numeric_handle_star_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_singleline_inputbox_keypad_numeric_handle_star_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_edit_singleline_inputbox_direct_input((UI_character_type) '*');
}


/*****************************************************************************
 * FUNCTION
 *  inline_singleline_inputbox_keypad_numeric_handle_pound_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_singleline_inputbox_keypad_numeric_handle_pound_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_edit_singleline_inputbox_direct_input((UI_character_type) '#');
}


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_singleline_inputbox_decimal_numeric_handle_pound_key_down
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_singleline_inputbox_decimal_numeric_handle_pound_key_down(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_buffer_type p;
    UI_character_type c;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_inline_singleline_inputbox_decimal_flag)
    {
        UI_editor_play_tone_invalid_data();
        return;
    }
    if (gui_single_line_input_box_test_first_position(&MMI_inline_singleline_inputbox))
    {
        inline_edit_singleline_inputbox_direct_input((UI_character_type) '0');
    }
    inline_edit_singleline_inputbox_direct_input((UI_character_type) '.');

    p = MMI_inline_singleline_inputbox.current_text_p;
    if (p != MMI_inline_singleline_inputbox.text)
    {
        UI_STRING_GET_PREVIOUS_CHARACTER(p, c);
        if (c == (UI_character_type) '.')
        {
            wgui_inline_singleline_inputbox_decimal_flag = 1;
        }
    }

}


/*****************************************************************************
 * FUNCTION
 *  inline_text_edit_set_RSK_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_text_edit_set_RSK_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((current_wgui_inline_item != NULL) && ((current_wgui_inline_item->flags & 0xff) == INLINE_ITEM_TYPE_TEXT_EDIT))
    {
        inline_text_edit_RSK_function = wgui_inline_edit_BACK_function;
    }
}

U8 wgui_inline_singleline_inputbox_input_mode_changed = 0;
U8 wgui_inline_singleline_inputbox_input_type = 0;


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_singleline_inputbox_change_input_mode_multitap_ABC
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_singleline_inputbox_change_input_mode_multitap_ABC(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_singleline_inputbox_input_type = INPUT_TYPE_ALPHANUMERIC_UPPERCASE;
    wgui_inline_singleline_inputbox_input_mode_changed = 1;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_singleline_inputbox_change_input_mode_multitap_abc
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_singleline_inputbox_change_input_mode_multitap_abc(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_singleline_inputbox_input_type = INPUT_TYPE_ALPHANUMERIC_LOWERCASE;
    wgui_inline_singleline_inputbox_input_mode_changed = 1;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_singleline_inputbox_change_input_mode_multitap_numeric
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_singleline_inputbox_change_input_mode_multitap_numeric(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_singleline_inputbox_input_type = INPUT_TYPE_NUMERIC;
    wgui_inline_singleline_inputbox_input_mode_changed = 1;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_singleline_inputbox_insert_symbol
 * DESCRIPTION
 *  
 * PARAMETERS
 *  symbol      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_singleline_inputbox_insert_symbol(UI_character_type symbol)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_edit_singleline_inputbox_direct_input(symbol);
}

extern void change_input_mode(void);

 
extern void change_singleline_input_mode(void);

/* CSD end */
extern void (*inputbox_change_mode) (void);


/*****************************************************************************
 * FUNCTION
 *  wgui_hide_inline_input_method
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_hide_inline_input_method(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    draw_title();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_display_inline_input_method
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_display_inline_input_method(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 iwidth, iheight, swidth, sheight;
    UI_string_type inputmode_string = (UI_string_type) "";
    stFontAttribute *f = &MMI_small_font;
    color_t c = *current_MMI_theme->shortcut_indicator_text_color;
// disable variable for compile	
//    UI_filled_area *bg = current_MMI_theme->shortcut_indicator_background_filler;
    S32 x1, y1, x2, y2, w, h;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_cancel_timer(wgui_display_inline_input_method);
    gui_cancel_timer(wgui_hide_inline_input_method);

    if (wgui_disable_inline_input_method_hints)
    {
        return;
    }

    gdi_layer_lock_frame_buffer();
    draw_title();

    switch (MMI_current_input_mode)
    {
        case 0:
            inputmode_string = (UI_string_type) get_string(WGUI_CATEGORY_ABC_STRING_ID);
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
        case 1:
            inputmode_string = (UI_string_type) get_string(WGUI_CATEGORY_abc_STRING_ID);
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
        case 2:
            inputmode_string = (UI_string_type) get_string(WGUI_CATEGORY_123_STRING_ID);
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
        default:
            wgui_inputbox_information_icon = NULL;
            break;
    }
    gui_measure_image(wgui_inputbox_information_icon, &iwidth, &iheight);
    gui_set_font(f);
    gui_measure_string(inputmode_string, &swidth, &sheight);
    w = swidth + iwidth + 6;
    if (w > UI_device_width)
    {
        w = UI_device_width;
    }
    h =  MMI_title_height;
    x1 = UI_device_width - w;
    x2 = UI_device_width - 1;
    y1 = MMI_title_y;
    y2 = y1 + MMI_title_height - 1;
    gdi_layer_set_clip(x1 + 2, y1 + 2, x2 - 2, y2 - 2);
//use title as background
   // gui_draw_filled_area(x1, y1, x2, y2, bg);
    gui_show_transparent_image(x1 + 2, y1 + (h >> 1) - (iheight >> 1), wgui_inputbox_information_icon, 0);
    gui_set_text_color(c);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + 2 + iwidth + 2 + swidth, y1 + (h >> 1) - (sheight >> 1));
    }
    else
    {
        gui_move_text_cursor(x1 + 2 + iwidth + 2, y1 + (h >> 1) - (sheight >> 1));
    }
    gui_set_line_height(sheight);
    gui_print_text(inputmode_string);
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(x1, y1, x2, y2);
   //disable the hide operate when show inline inputmethod icon
   //gui_start_timer(INLINE_INPUT_METHOD_HIDE_WAIT, wgui_hide_inline_input_method);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_handle_inline_singleline_inputbox_change_input_mode
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_handle_inline_singleline_inputbox_change_input_mode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    disable_active_multitap();
    /* change_input_mode(); */
    change_singleline_input_mode();
    wgui_display_inline_input_method();
}


/*****************************************************************************
 * FUNCTION
 *  change_inline_singleline_inputbox_input_mode
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void change_inline_singleline_inputbox_input_mode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_edit_change_singleline_inputbox_mode((U8) MMI_current_input_mode);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_singleline_inputbox_get_UCS2_count
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 wgui_inline_singleline_inputbox_get_UCS2_count(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (MMI_inline_singleline_inputbox.UCS2_count);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_singleline_inputbox_validation_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  text            [IN]        
 *  cursor          [IN]        
 *  text_length     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_singleline_inputbox_validation_callback(UI_buffer_type text, UI_buffer_type cursor, S32 text_length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_inputbox_validation_callback != NULL)
    {
        wgui_inputbox_validation_callback(text, cursor, text_length);
    }
}

/* History implementation for inline text edit  */

typedef struct _inline_text_edit_history
{
    U16 history_ID;
    S16 text_length;
    S16 allocated_length;
    S16 available_length;
    S16 current_position;
    S16 text_offset_x;
    S16 input_type;
    S16 dummy;  /* Added for history issue on hardware */
    U32 flags;
#if defined(__MMI_TOUCH_SCREEN__)
    S16 vk_lang_type;
#endif 
} inline_text_edit_history;


/*****************************************************************************
 * FUNCTION
 *  inline_text_edit_get_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]         
 *  input_type          [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 *inline_text_edit_get_history(U8 *history_buffer, S16 input_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        U16 hID = (U16) (INLINE_TEXT_EDIT_HISTORY_ID | 0x8000);
        inline_text_edit_history *h = (inline_text_edit_history*) history_buffer;

        h->history_ID = hID;
        h->text_length = (S16) MMI_inline_singleline_inputbox.text_length;
        h->allocated_length = (S16) MMI_inline_singleline_inputbox.allocated_length;
        h->available_length = (S16) MMI_inline_singleline_inputbox.available_length;
        h->current_position =
            (S16) (MMI_inline_singleline_inputbox.current_text_p - MMI_inline_singleline_inputbox.text);
        h->text_offset_x = (S16) MMI_inline_singleline_inputbox.text_offset_x;
        h->input_type = (S16) input_type;
        h->flags = MMI_inline_singleline_inputbox.flags;
    #if defined(__MMI_TOUCH_SCREEN__)
        if (GUI_VIRTUAL_KEYBOARD_MAX_LANG == mmi_pen_editor_history_vk_lang_type())
        {
            h->vk_lang_type = MMI_virtual_keyboard.lang_type;
        }
        else
        {
            h->vk_lang_type = mmi_pen_editor_history_vk_lang_type();
        }
    #endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    }
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  inline_text_edit_set_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [?]         
 *  input_type          [?]         
 * RETURNS
 *  
 *****************************************************************************/
U8 inline_text_edit_set_history(U16 history_ID, U8 *history_buffer, S16 *input_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        inline_text_edit_history *h = (inline_text_edit_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            /*
 
             * *    because the text_length will be set correctly in handle_inline_text_edit->create_inline_edit_singleline_inputbox_set_buffer.
             * *    if restored it, will cause MMI_inline_single_line_inputbox have wrong text length(asyn with text buffer)
             * *    for example,
             * *       (1)in inline editor screen and highlighted at first item(first line will lead user to full screen editor, and second item is inline single input box)
             * *       (2)charger in --> popup a new window and add current screen related parameters into history including text_length
             * *       (3)and the text_length maybe not correct because text_length of input boxes(inline single, single, multi line, or EMS) will not be reset
             * *       (4)while popup dispeared, go back to (1)
             * *       (5)press arrow down to go to next item, inline single line input box
             * *       (6)handle_inline_text_edit->create_inline_edit_singleline_inputbox_set_buffer will calculate the length from text buffer
             * *       (7)inline_text_edit_set_history will restore the text length from history
             * *       (8)inline_text_edit_set_history caused text_length not sync with current input box text buffer
             * *            Therefore, comment out MMI_inline_singleline_inputbox.text_length=h->text_length;
             */
             
            MMI_inline_singleline_inputbox.text_length = h->text_length;
            MMI_inline_singleline_inputbox.allocated_length = h->allocated_length;
            MMI_inline_singleline_inputbox.available_length = h->available_length;
            MMI_inline_singleline_inputbox.current_text_p =
                (UI_buffer_type) (MMI_inline_singleline_inputbox.text + h->current_position);
            MMI_inline_singleline_inputbox.text_offset_x = h->text_offset_x;
            /* CSD end */
            *input_type = h->input_type;
            MMI_inline_singleline_inputbox.flags = h->flags;
        #if defined(__MMI_TOUCH_SCREEN__)
            wgui_setup_virtual_keyboard(h->vk_lang_type);
        #endif 
            return (1);
        }
    }
    return (0);
}

/*****************************************************************************
 * FUNCTION
 *  inline_text_edit_handle_symbol_picker_star_key
 * DESCRIPTION
 *  Show symbols screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_text_edit_handle_symbol_picker_star_key(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    disable_active_multitap();
    MMI_current_input_ext_type |= INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL;
    EntrySymbolPickerScreen();
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_text_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_text_edit(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 l, xx;
    S16 input_type;
    S16 input_type_flags;
    U8 saved_MMI_current_input_mode;
    U32 saved_inputbox_flags;
    U8 override_history_input_type = 0;
    S16 saved_input_type;
    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) item;
    U8 plus_character_handling = 0;

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_handwriting_area_struct stroke_area;
    mmi_pen_handwriting_area_struct ext_stroke;
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(flags);
     
    set_current_input_box_type(EDITOR_INLINE_TEXT);
    wgui_inline_singleline_inputbox_buffer_size = i->buffer_size;
    if (i->input_type & INPUT_TYPE_PLUS_CHARACTER_HANDLING)
    {
        plus_character_handling = 1;
    }
    wgui_current_inline_item_type = INLINE_ITEM_TYPE_TEXT_EDIT;
     
     
    if (!r2lMMIFlag)
    {
        if (MMI_fixed_icontext_menuitem.text_x == 2)
        {
            xx = 0;
        }
        else
        {
            xx = MMI_fixed_icontext_menuitem.text_x;
        }
    }
    else
    {
        xx = 1;
    }
    /* CSD end */
     
    if (flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = 0;
    }
    clear_inline_item_keys = clear_inline_singleline_inputbox_keys;
    register_default_hide_softkeys();
    complete_inline_item_edit = handle_inline_text_edit_complete;
    current_wgui_inline_text_edit_item = i;
    current_wgui_inline_text_edit_text_p = text_p;
    set_left_softkey_label(get_string(STR_GLOBAL_OK));
    gui_strcpy((UI_string_type) wgui_inline_text_edit_buffer, (UI_string_type) i->buffer);
    l = gui_strlen((UI_string_type) wgui_inline_text_edit_buffer);
    create_inline_edit_singleline_inputbox_set_buffer(
        (UI_string_type) wgui_inline_text_edit_buffer,
        i->buffer_size,
        l,
        l);
    switch (flags & INLINE_ITEM_JUSTIFY_MASK)
    {
        case INLINE_ITEM_RIGHT_JUSTIFY:
            MMI_inline_singleline_inputbox.flags |= UI_SINGLE_LINE_INPUT_BOX_RIGHT_JUSTIFY;
            break;

        case INLINE_ITEM_CENTER_JUSTIFY:
            MMI_inline_singleline_inputbox.flags |= UI_SINGLE_LINE_INPUT_BOX_CENTER_JUSTIFY;
            break;
    }
    if ((flags & INLINE_ITEM_BOUNDARY_MASK) == INLINE_ITEM_BOUNDARY_NEVER)
    {
        gui_set_single_line_input_box_theme(
            &MMI_inline_singleline_inputbox,
            &wgui_inline_singleline_inputbox_no_boundary_theme);
    }
     
    if (!r2lMMIFlag)
    {
         
    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        if (line_draw)
        {
            inline_edit_resize_singleline_inputbox(width - xx - 2, height - 1);
        }
        else
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
             
        {
             
            /* resize the inputbox so that the border can be drawn to have the
               3D effect in case of default text effect is enabled. */
        #if defined(__MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__) && (defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__))       /* 122805 3D inline edit */
            if (IsInlineItemDefaultTextEnable())
            {
                inline_edit_resize_singleline_inputbox(width - xx - 4, height - 4);
            }
            else
        #endif /* defined(__MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__) && (defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__)) */ 
                 
                inline_edit_resize_singleline_inputbox(width - xx, height);
        }
    }
    else
    {
         
    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        if (line_draw)
        {
            inline_edit_resize_singleline_inputbox(width - MMI_fixed_icontext_menuitem.text_x - 6, height - 1);
        }
        else
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
             
        {
             
            /* resize the inputbox so that the border can be drawn to have the
               3D effect in case of default text effect is enabled. */
 
        #if defined __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ && (defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__))
  
            if (IsInlineItemDefaultTextEnable())
            {
                inline_edit_resize_singleline_inputbox(width - MMI_fixed_icontext_menuitem.text_x - 4, height - 4);
            }
            else
        #endif /* defined __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ && (defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__)) */ 
                 
                inline_edit_resize_singleline_inputbox(width - MMI_fixed_icontext_menuitem.text_x, height);
        }
    }
    //CSD end
     
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    if (line_draw)
    {
        inline_edit_move_singleline_inputbox(x + xx + 1, y + 1);
    }
    else
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
         
         
        /* move the inputbox at correct location, in case of default text effect
           is enabled. */
#if defined(__MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__) && (defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__))       /* 122805 3D inline edit */
    if (IsInlineItemDefaultTextEnable())
    {
        inline_edit_move_singleline_inputbox(x + xx + 2, y + 2);
    }
    else
#endif /* defined(__MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__) && (defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__)) */ 
         
    inline_edit_move_singleline_inputbox(x + xx, y);
    register_inline_singleline_inputbox_keys();
    input_type_flags = (S16) (i->input_type & (~INPUT_TYPE_MASK));
    input_type = (S16) (i->input_type & INPUT_TYPE_MASK);

    if (input_type_flags & INPUT_TYPE_USE_ONLY_ENGLISH_MODES)
    {
        set_english_only_multitap();
        MMI_allow_only_english_input_modes = 1;
    }
    else
    {
        set_normal_multitap();
    }
    saved_input_type = input_type;
    switch (input_type & INPUT_TYPE_MASK)
    {
        case INPUT_TYPE_NUMERIC_PASSWORD:
        case INPUT_TYPE_SAT_NUMERIC_PASSWORD:
            override_history_input_type = 1;
            break;
    }
    inline_text_edit_set_history(INLINE_TEXT_EDIT_HISTORY_ID, history_buffer, &input_type);
    saved_MMI_current_input_mode = MMI_current_input_mode;
    saved_inputbox_flags = MMI_inline_singleline_inputbox.flags;
    if (override_history_input_type)
    {
        input_type = saved_input_type;
    }

    /* Get input mode from "Input Methods" list selection */
    wgui_change_inputbox_mode_multitap_ABC = wgui_inline_singleline_inputbox_change_input_mode_multitap_ABC;
    wgui_change_inputbox_mode_multitap_abc = wgui_inline_singleline_inputbox_change_input_mode_multitap_abc;
    wgui_change_inputbox_mode_mode_numeric = wgui_inline_singleline_inputbox_change_input_mode_multitap_numeric;
    wgui_inputbox_insert_symbol = wgui_inline_singleline_inputbox_insert_symbol;
    if (wgui_inline_singleline_inputbox_input_mode_changed)
    {
        input_type = wgui_inline_singleline_inputbox_input_type;
        wgui_inline_singleline_inputbox_input_mode_changed = 0;
    }
    MMI_current_input_type = input_type;

    wgui_inline_singleline_inputbox_decimal_input_mode_flag = 0;
    wgui_inputbox_UCS2_count_function = wgui_inline_singleline_inputbox_get_UCS2_count;
    if (input_type & INPUT_TYPE_USE_ENCODING_BASED_LENGTH)
    {
        MMI_inline_singleline_inputbox.flags |= UI_SINGLE_LINE_INPUT_BOX_USE_ENCODING_BASED_LENGTH;
        if (input_type & INPUT_TYPE_ONE_LESS_CHARACTER)
        {
            MMI_inline_singleline_inputbox.flags |= UI_SINGLE_LINE_INPUT_BOX_ONE_LESS_CHARACTER;
            UI_TEST_UCS2_COUNT_SET_LENGTH_TYPE2(
                MMI_inline_singleline_inputbox.UCS2_count,
                MMI_inline_singleline_inputbox.allocated_length,
                MMI_inline_singleline_inputbox.available_length);
        }
        else
        {
            UI_TEST_UCS2_COUNT_SET_LENGTH(
                MMI_inline_singleline_inputbox.UCS2_count,
                MMI_inline_singleline_inputbox.allocated_length,
                MMI_inline_singleline_inputbox.available_length);
        }
    }
    if (plus_character_handling)
    {
        MMI_inline_singleline_inputbox.flags |= UI_SINGLE_LINE_INPUT_BOX_PLUS_CHARACTER_HANDLING;
    }

    /* Input type currently cannot be changed by the user in inline edit */
    switch (input_type & INPUT_TYPE_MASK)
    {
        case INPUT_TYPE_NUMERIC_CHANGEABLE:
            set_inline_edit_singleline_inputbox_mask(0);
            inline_edit_change_singleline_inputbox_mode(INPUT_MODE_123);
            SetKeyHandler(wgui_handle_inline_singleline_inputbox_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
            SetKeyHandler(inline_text_edit_handle_symbol_picker_star_key, KEY_STAR, KEY_EVENT_DOWN);
            inputbox_change_mode = change_inline_singleline_inputbox_input_mode;
            MMI_current_input_mode = INPUT_MODE_123;
            break;

        case INPUT_TYPE_NUMERIC:
            set_inline_edit_singleline_inputbox_mask(0);
            inline_edit_change_singleline_inputbox_mode(INPUT_MODE_123);
            MMI_current_input_mode = INPUT_MODE_123;
            break;

        case INPUT_TYPE_DECIMAL_NUMERIC:
            wgui_inline_singleline_inputbox_decimal_flag = 0;
            if (l > 0)
            {
                S32 k;
                UI_buffer_type t = *text_p;
                UI_character_type c;

                for (k = 0; k < l; k++)
                {
                    UI_STRING_GET_NEXT_CHARACTER(t, c);
                    if (c == (UI_character_type) '.')
                    {
                        wgui_inline_singleline_inputbox_decimal_flag = 1;
                        break;
                    }
                }
            }
            set_inline_edit_singleline_inputbox_mask(0);
            inline_edit_change_singleline_inputbox_mode(INPUT_MODE_123);
            MMI_current_input_mode = INPUT_MODE_123;
            SetKeyHandler(
                wgui_inline_singleline_inputbox_decimal_numeric_handle_pound_key_down,
                KEY_POUND,
                KEY_EVENT_DOWN);
            wgui_inline_singleline_inputbox_decimal_input_mode_flag = 1;
            break;

        case INPUT_TYPE_KEYPAD_NUMERIC:
            set_inline_edit_singleline_inputbox_mask(0);
            inline_edit_change_singleline_inputbox_mode(INPUT_MODE_123);
            SetKeyHandler(inline_singleline_inputbox_keypad_numeric_handle_star_key_down, KEY_STAR, KEY_EVENT_DOWN);
            SetKeyHandler(inline_singleline_inputbox_keypad_numeric_handle_pound_key_down, KEY_POUND, KEY_EVENT_DOWN);
            MMI_current_input_mode = INPUT_MODE_123;
            break;

        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE:
            set_inline_edit_singleline_inputbox_mask(0);
            inline_edit_change_singleline_inputbox_mode(INPUT_MODE_MULTITAP_UPPERCASE_ABC);
            SetKeyHandler(wgui_handle_inline_singleline_inputbox_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
            SetKeyHandler(inline_text_edit_handle_symbol_picker_star_key, KEY_STAR, KEY_EVENT_DOWN);
            inputbox_change_mode = change_inline_singleline_inputbox_input_mode;
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_ABC;
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE:
            set_inline_edit_singleline_inputbox_mask(0);
            inline_edit_change_singleline_inputbox_mode(1);
            SetKeyHandler(wgui_handle_inline_singleline_inputbox_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
            SetKeyHandler(inline_text_edit_handle_symbol_picker_star_key, KEY_STAR, KEY_EVENT_DOWN);
            inputbox_change_mode = change_inline_singleline_inputbox_input_mode;
            MMI_current_input_mode = 1;
            break;

        case INPUT_TYPE_ALPHANUMERIC_SENTENCECASE:
            set_inline_edit_singleline_inputbox_mask(0);
            inline_edit_change_singleline_inputbox_mode(INPUT_MODE_MULTITAP_LOWERCASE_ABC);
            SetKeyHandler(wgui_handle_inline_singleline_inputbox_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
            inputbox_change_mode = change_inline_singleline_inputbox_input_mode;
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_ABC;
            break;

        case INPUT_TYPE_NUMERIC_PASSWORD:
            set_inline_edit_singleline_inputbox_mask(1);
            inline_edit_change_singleline_inputbox_mode(INPUT_MODE_123);
            MMI_current_input_mode = INPUT_MODE_123;
            break;

        case INPUT_TYPE_ALPHANUMERIC_PASSWORD:
            set_inline_edit_singleline_inputbox_mask(1);
            inline_edit_change_singleline_inputbox_mode(INPUT_MODE_MULTITAP_LOWERCASE_ABC);
            SetKeyHandler(wgui_handle_inline_singleline_inputbox_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
            inputbox_change_mode = change_inline_singleline_inputbox_input_mode;
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_ABC;
            break;

        case INPUT_TYPE_PHONE_NUMBER:
            set_inline_edit_singleline_inputbox_mask(0);
            inline_edit_change_singleline_inputbox_mode(INPUT_MODE_123);
            /* Handling of Phone number input   */
             
        #ifdef __MMI_MULTITAP_KEY_0__
            SetKeyHandler(inline_edit_phone_number_input_handle_star_key_down, KEY_STAR, KEY_EVENT_DOWN);
        #else /* __MMI_MULTITAP_KEY_0__ */ 
            gui_create_multitap_input(
                &wgui_inline_text_edit_multitap_star_key,
                0,
                0,
                0,
                0,
                (UI_string_type) wgui_inline_edit_phone_number_star_key_string);
            gui_set_multitap_input_callbacks(
                &wgui_inline_text_edit_multitap_star_key,
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
            SetKeyHandler(inline_edit_phone_number_input_handle_multitap_star_key_down, KEY_STAR, KEY_EVENT_DOWN);
            SetKeyHandler(inline_edit_phone_number_input_handle_multitap_star_key_up, KEY_STAR, KEY_EVENT_UP);
        #endif /* __MMI_MULTITAP_KEY_0__ */ 
            /* CSD end */
            SetKeyHandler(inline_edit_phone_number_input_handle_hash_key_down, KEY_POUND, KEY_EVENT_DOWN);
            register_key_down_handler(inline_edit_phone_number_input_numeric_key_handler);
        #if(ENABLE_DIALER_ZERO_KEY_MULTITAP)
            gui_create_multitap_input(
                &wgui_inline_text_edit_multitap_zero_key,
                0,
                0,
                0,
                0,
                (UI_string_type) wgui_inline_edit_phone_number_zero_key_string);
            gui_set_multitap_input_callbacks(
                &wgui_inline_text_edit_multitap_zero_key,
                inline_edit_singleline_inputbox_multitap_input,
                inline_edit_singleline_inputbox_multitap_input_complete);
        #ifdef __MMI_MULTITAP_KEY_0__
            SetKeyHandler(inline_edit_phone_number_input_handle_zero_key_long_press, KEY_0, KEY_EVENT_LONG_PRESS);
        #else /* __MMI_MULTITAP_KEY_0__ */ 
            SetKeyHandler(inline_edit_phone_number_input_handle_multitap_zero_key_down, KEY_0, KEY_EVENT_DOWN);
            SetKeyHandler(inline_edit_phone_number_input_handle_multitap_zero_key_up, KEY_0, KEY_EVENT_UP);
        #endif /* __MMI_MULTITAP_KEY_0__ */ 
        #endif /* (ENABLE_DIALER_ZERO_KEY_MULTITAP) */ 
            wgui_inline_text_edit_active_multitap = WGUI_INLINE_TEXT_EDIT_MULTITAP_NONE;
            MMI_current_input_mode = INPUT_MODE_123;
            break;

        default:
            set_inline_edit_singleline_inputbox_mask(0);
            inline_edit_change_singleline_inputbox_mode(INPUT_MODE_123);
            MMI_current_input_mode = INPUT_MODE_123;
            break;
    }
    if (!override_history_input_type && (saved_inputbox_flags & UI_SINGLE_LINE_INPUT_BOX_MASK_CHARACTERS))
    {
        MMI_inline_singleline_inputbox.flags |= UI_SINGLE_LINE_INPUT_BOX_MASK_CHARACTERS;
        if ((saved_input_type & INPUT_TYPE_MASK) != INPUT_TYPE_ALPHANUMERIC_PASSWORD)
        {
            ClearKeyHandler(KEY_POUND, KEY_EVENT_DOWN);
        }
    }
     
    /* shows the border with 3D effect in case of inline text edit item, when the
       default text effect is enabled. */
#if defined(__MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__) && (defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__))       /* 122805 3D inline edit */
    if (IsInlineItemDefaultTextEnable())
    {
        inline_text_edit_show_background(&MMI_inline_singleline_inputbox);
    }
#endif /* defined(__MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__) && (defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_220X176__)) */ 
     
    show_inline_singleline_inputbox();
    inline_text_edit_RSK_label_clear = 0;
    if (wgui_inline_list_menu_changed)
    {
        if (wgui_inline_list_menu_disable_done)
        {
            inline_text_edit_RSK_label_string = (UI_string_type) get_string(STR_GLOBAL_BACK);
            inline_text_edit_RSK_label_icon = NULL;
        }
        else
        {
            inline_text_edit_RSK_label_string = NULL;
            inline_text_edit_RSK_label_icon = NULL;
        }
    }
    else
    {
        inline_text_edit_RSK_label_string = wgui_inline_edit_get_RSK_string();
        inline_text_edit_RSK_label_icon = NULL;
    }
    set_right_softkey_label(inline_text_edit_RSK_label_string);
    set_right_softkey_icon(inline_text_edit_RSK_label_icon);
    register_right_softkey_handler();
#ifdef __MMI_WITH_C_KEY__
    SetKeyHandler(handle_inline_text_edit_right_softkey_down, KEY_CLEAR, KEY_EVENT_DOWN);
    SetKeyHandler(handle_inline_text_edit_right_softkey_long_press, KEY_CLEAR, KEY_LONG_PRESS);
    set_right_softkey_function(handle_inline_text_edit_right_softkey_up, KEY_EVENT_UP);
#else /* __MMI_WITH_C_KEY__ */ 

#ifdef __MMI_FULL_KEYPAD__
    SetKeyHandler(handle_inline_text_edit_right_softkey_down, KEY_BACKSPACE1, KEY_EVENT_DOWN);
    SetKeyHandler(handle_inline_text_edit_right_softkey_up, KEY_BACKSPACE1, KEY_EVENT_UP);
    SetKeyHandler(handle_inline_text_edit_right_softkey_long_press, KEY_BACKSPACE1, KEY_EVENT_LONG_PRESS);
#endif

    set_right_softkey_function(handle_inline_text_edit_right_softkey_down, KEY_EVENT_DOWN);
    set_right_softkey_function(handle_inline_text_edit_right_softkey_long_press, KEY_LONG_PRESS);
#endif /* __MMI_WITH_C_KEY__ */ 
    inline_edit_register_singleline_inputbox_input_callback(handle_inline_text_edit_input);
    /* inline_edit_register_singleline_inputbox_navigate_callback(handle_inline_text_edit_navigate); */
    inline_text_edit_RSK_function = wgui_inline_edit_BACK_function;
    handle_inline_text_edit_input();
    if (gui_single_line_input_box_get_text_length(&MMI_inline_singleline_inputbox) <= 0)
    {
        //#if defined(__MMI_WITH_C_KEY__)
        //     SetKeyHandler(handle_inline_text_edit_right_softkey_up, KEY_CLEAR, KEY_EVENT_UP);
        //#else
        set_right_softkey_function(handle_inline_text_edit_right_softkey_up, KEY_EVENT_UP);
        // #endif
        if (wgui_inputbox_empty_callback != NULL)
        {
            wgui_inputbox_empty_callback();
        }
    }
    else if (wgui_inputbox_not_empty_callback != NULL)
    {
        wgui_inputbox_not_empty_callback();
    }
    wgui_inputbox_validation_callback = i->validation_callback;
    MMI_inline_singleline_inputbox.validation_callback = wgui_inline_singleline_inputbox_validation_callback;
    MMI_inline_singleline_inputbox.validation_callback(
                                    MMI_inline_singleline_inputbox.text,
                                    MMI_inline_singleline_inputbox.current_text_p,
                                    MMI_inline_singleline_inputbox.text_length);
    SetKeyHandler(inline_text_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_text_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_text_edit_handle_down_arrow, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    SetKeyHandler(inline_text_edit_handle_up_arrow, KEY_VOL_UP, KEY_EVENT_DOWN);
    redraw_left_softkey();
    redraw_right_softkey();
    redraw_current_inline_item = redraw_inline_singleline_inputbox;
    current_wgui_inline_text_edit_item->UCS2_count = MMI_inline_singleline_inputbox.UCS2_count;
    test_change_input_mode = inline_singleline_inputbox_test_change_input_mode;

    if (i->custom_setup_function != NULL)
    {
        i->custom_setup_function();
    }

#if  !defined(__MMI_MAINLCD_220X176__)	
    wgui_display_inline_input_method();
#endif
    //gui_start_timer(INLINE_INPUT_METHOD_DISPLAY_WAIT, wgui_display_inline_input_method);
    UI_UNUSED_PARAMETER(input_type_flags);
    UI_UNUSED_PARAMETER(saved_MMI_current_input_mode);
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    ext_stroke.x1 = 0;
    ext_stroke.y1 = 0;
    ext_stroke.x2 = UI_device_width - 1;
    ext_stroke.y2 = UI_device_height - 1;
#if 0
#else /* 0 */ 
    stroke_area.x1 = MMI_content_x;
    stroke_area.x2 = MMI_content_x + MMI_content_width - MMI_fixed_list_menu.vbar.width;
    stroke_area.y1 = MMI_title_y;
    stroke_area.y2 = MMI_title_y + MMI_content_height + MMI_title_height;
#endif /* 0 */ 
    mmi_pen_editor_setup_input_box(
        &stroke_area,
        &ext_stroke,
        MMI_current_input_type,
        MMI_FALSE,
        PEN_EDITOR_INLINE_TEXT);
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineTextEditCustomFunction
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineTextEditCustomFunction(InlineItem *item, void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) item->item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i->custom_setup_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemTextEdit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item            [?]         
 *  buffer          [?]         
 *  buffer_size     [IN]        
 *  input_type      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemTextEdit(InlineItem *item, U8 *buffer, S32 buffer_size, S16 input_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i;
    UI_buffer_type mask_buffer;
    S16 t = input_type & INPUT_TYPE_MASK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_inline_text_edit_items >= MAX_INLINE_TEXT_EDIT_ITEMS)
    {
        return;
    }
    item->item = (void*)&wgui_inline_text_edit_items[n_inline_text_edit_items];
    item->flags = INLINE_ITEM_TYPE_TEXT_EDIT;
    mask_buffer = inline_text_edit_mask_buffers[n_inline_text_edit_items];
    n_inline_text_edit_items++;
    i = (wgui_inline_item_text_edit*) item->item;
    i->max_lines = 0;
    i->buffer = buffer;
    i->buffer_size = buffer_size;
    i->flags = 0;
    i->input_type = input_type;
    item->text_p = i->buffer;
    item->handler = handle_inline_text_edit;
    i->edit_complete_callback = UI_dummy_edit_complete_handler;
    i->validation_callback = UI_dummy_validation_function;
    i->custom_setup_function = NULL;
    if ((t == INPUT_TYPE_NUMERIC_PASSWORD) || (t == INPUT_TYPE_ALPHANUMERIC_PASSWORD) ||
        (t == INPUT_TYPE_SAT_NUMERIC_PASSWORD))
    {
        inline_edit_prepare_mask_buffer(buffer, mask_buffer);
        i->mask_buffer = mask_buffer;
        item->text_p = mask_buffer;
    }
    /* UCS2_count==-1 means this field hasn't been edited */
    i->UCS2_count = -1;
}


/*****************************************************************************
 * FUNCTION
 *  ReConfigureInlineItemTextEdit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item            [?]         
 *  buffer          [?]         
 *  buffer_size     [IN]        
 *  input_type      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ReConfigureInlineItemTextEdit(InlineItem *item, U8 *buffer, S32 buffer_size, S16 input_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i;
    UI_buffer_type mask_buffer;
    S16 t = input_type & INPUT_TYPE_MASK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((item->flags & 0xff) != INLINE_ITEM_TYPE_TEXT_EDIT)
    {
        return;
    }
    i = (wgui_inline_item_text_edit*) item->item;
    mask_buffer = i->mask_buffer;
    i->max_lines = 0;
    i->buffer = buffer;
    i->buffer_size = buffer_size;
    i->flags = 0;
    i->input_type = input_type;
    item->text_p = i->buffer;
    i->edit_complete_callback = UI_dummy_edit_complete_handler;
    i->custom_setup_function = NULL;
    if ((t == INPUT_TYPE_NUMERIC_PASSWORD) || (t == INPUT_TYPE_ALPHANUMERIC_PASSWORD) ||
        (t == INPUT_TYPE_SAT_NUMERIC_PASSWORD))
    {
        inline_edit_prepare_mask_buffer(buffer, mask_buffer);
        i->mask_buffer = mask_buffer;
        item->text_p = mask_buffer;
    }
}


/*****************************************************************************
 * FUNCTION
 *  RegisterInlineTextEditCompleteHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterInlineTextEditCompleteHandler(InlineItem *item, void (*f) (U8 *string, S32 buffer_size))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = (wgui_inline_item_text_edit*) item->item;
    if (f != NULL)
    {
        i->edit_complete_callback = f;
    }
}


/*****************************************************************************
 * FUNCTION
 *  RegisterInlineTextEditValidationFunction
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterInlineTextEditValidationFunction(InlineItem *item, void (*f) (U8 *buffer, U8 *cursor, S32 text_length))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = (wgui_inline_item_text_edit*) item->item;
    if (f != NULL)
    {
        i->validation_callback = f;
    }
}

 
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemTextEditDefaultItemNameIfEmpty
 * DESCRIPTION
 *  Sets the pointer item->text_p to point to default item text
 *  if the buffer is empty
 * PARAMETERS
 *  item                [IN]        Is the pointer to InlineItem structure.
 *  default_text_id     [IN]        Is the string Id of the default text.
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemTextEditDefaultItemNameIfEmpty(InlineItem *item, U16 default_text_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i;
    S32 sw, sh;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = (wgui_inline_item_text_edit*) item->item;
    i->default_item_text = (U8*) get_string(default_text_id);
    gui_measure_string((UI_string_type) i->buffer, &sw, &sh);
    if ((IsInlineItemDefaultTextEnable()) && (sw == 0))
    {
        item->text_p = i->default_item_text;
    }
}


/*****************************************************************************
 * FUNCTION
 *  EnableInlineItemDefaultText
 * DESCRIPTION
 *  Sets the global flag variable to 1.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EnableInlineItemDefaultText(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_enable_inline_item_default_text = 1;
}


/*****************************************************************************
 * FUNCTION
 *  DisableInlineItemDefaultText
 * DESCRIPTION
 *  Sets the global flag variable to 0.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DisableInlineItemDefaultText(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_enable_inline_item_default_text = 0;
}


/*****************************************************************************
 * FUNCTION
 *  IsInlineItemDefaultTextEnable
 * DESCRIPTION
 *  Check whether the global flag is on or off.
 * PARAMETERS
 *  void
 * RETURNS
 *  1 if the flag is set otherwise 0.
 *****************************************************************************/
U8 IsInlineItemDefaultTextEnable(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (wgui_enable_inline_item_default_text == 1);
}

#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
 

/* Displaying inline text edit as a menu item in the fixed list menu */

single_line_input_box wgui_inline_text_edit_display_inputbox;


/*****************************************************************************
 * FUNCTION
 *  wgui_show_inline_text_edit_list_menuitem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  width       [IN]        
 *  height      [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_inline_text_edit_list_menuitem(S32 x1, S32 y1, S32 width, S32 height, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 xx, l;
    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) item->item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_fixed_icontext_menuitem.text_x == 2)
    {
        xx = -1;
    }
    else
    {
        xx = MMI_fixed_icontext_menuitem.text_x;
    }
    if (item->flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    gdi_layer_lock_frame_buffer();
    /* add check to use mask buffer in case of passwords here   */
    l = gui_strlen((UI_string_type) i->buffer);
#ifdef __UCS2_ENCODING
    gui_create_single_line_input_box_set_buffer_first_position(
        &wgui_inline_text_edit_display_inputbox,
        MMI_singleline_inputbox_x,
        MMI_singleline_inputbox_y,
        MMI_singleline_inputbox_width,
        MMI_singleline_inputbox_height,
        (UI_string_type) i->buffer,
        i->buffer_size,
        (l + 1) * 2,
        0);
#endif /* __UCS2_ENCODING */ 
#ifdef __ASCII
    gui_create_single_line_input_box_set_buffer_first_position(
        &wgui_inline_text_edit_display_inputbox,
        MMI_singleline_inputbox_x,
        MMI_singleline_inputbox_y,
        MMI_singleline_inputbox_width,
        MMI_singleline_inputbox_height,
        (UI_string_type) i->buffer,
        i->buffer_size,
        l + 1,
        0);
#endif /* __ASCII */ 
    wgui_inline_text_edit_display_inputbox.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW;
    switch (i->input_type & INPUT_TYPE_MASK)
    {
        case INPUT_TYPE_NUMERIC_PASSWORD:
        case INPUT_TYPE_ALPHANUMERIC_PASSWORD:
            wgui_inline_text_edit_display_inputbox.flags |= UI_SINGLE_LINE_INPUT_BOX_MASK_CHARACTERS;
            break;
    }
    switch (item->flags & INLINE_ITEM_JUSTIFY_MASK)
    {
        case INLINE_ITEM_RIGHT_JUSTIFY:
            wgui_inline_text_edit_display_inputbox.flags |= UI_SINGLE_LINE_INPUT_BOX_RIGHT_JUSTIFY;
            break;
        case INLINE_ITEM_CENTER_JUSTIFY:
            wgui_inline_text_edit_display_inputbox.flags |= UI_SINGLE_LINE_INPUT_BOX_CENTER_JUSTIFY;
            break;
    }

    wgui_inline_text_edit_display_inputbox.flags |= UI_SINGLE_LINE_INPUT_BOX_TRUNCATE_CONTENT_NO_BRIEF;  

    if ((item->flags & INLINE_ITEM_BOUNDARY_MASK) == INLINE_ITEM_BOUNDARY_ALWAYS)
    {
        gui_set_single_line_input_box_theme(
            &wgui_inline_text_edit_display_inputbox,
            &wgui_inline_singleline_inputbox_unfocussed_theme);
        wgui_inline_text_edit_display_inputbox.text_font = &MMI_medium_font;
    }
    gui_resize_single_line_input_box(&wgui_inline_text_edit_display_inputbox, width - 1 - xx, height);
    gui_move_single_line_input_box(&wgui_inline_text_edit_display_inputbox, x1 + xx, y1);
    gui_show_single_line_input_box(&wgui_inline_text_edit_display_inputbox);
    redraw_left_softkey();
    redraw_right_softkey();
    gdi_layer_unlock_frame_buffer();
}

/*----------------------------------------------------------------------------
   Inline text edit (Full screen) implementation
----------------------------------------------------------------------------*/

/* History implementation for inline text edit  */

typedef multiline_inputbox_category_history inline_full_screen_edit_history;


/*****************************************************************************
 * FUNCTION
 *  inline_full_screen_edit_get_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]         
 *  input_type          [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 *inline_full_screen_edit_get_history(U8 *history_buffer, S16 input_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_multiline_inputbox_category_history(INLINE_FULL_SCREEN_EDIT_HISTORY_ID, history_buffer, input_type);
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  inline_full_screen_edit_set_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [?]         
 *  input_type          [?]         
 * RETURNS
 *  
 *****************************************************************************/
U8 inline_full_screen_edit_set_history(U16 history_ID, U8 *history_buffer, S16 *input_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (set_multiline_inputbox_category_history(INLINE_FULL_SCREEN_EDIT_HISTORY_ID, history_buffer, input_type));
}

 


/*****************************************************************************
 * FUNCTION
 *  inline_multi_line_edit_get_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]         
 *  input_type          [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 *inline_multi_line_edit_get_history(U8 *history_buffer, S16 input_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_multiline_inputbox_category_history(INLINE_MULTI_LINE_EDIT_HISTORY_ID, history_buffer, input_type);
    return (history_buffer);
}

 


/*****************************************************************************
 * FUNCTION
 *  redraw_inline_full_screen_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_inline_full_screen_edit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* micha1111, #219 */
#ifdef __MMI_DEFAULT_THEME_3__
    color_t c = {206, 206, 206, 100};
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();

    /* micha1111 Chicago Feature */
#ifdef __MMI_DEFAULT_THEME_3__
    gdi_layer_reset_clip();
    gui_fill_rectangle(0, 0, UI_device_width - 1, UI_device_height - 1, c);
#else /* __MMI_DEFAULT_THEME_3__ */ 
    if (wgui_is_wallpaper_on_bottom() == MMI_TRUE)
    {
        gdi_draw_solid_rect(0, 0, UI_device_width - 1, UI_device_height - 1, GDI_COLOR_TRANSPARENT);
    }
    else
    {
        clear_screen();
    }
#endif /* __MMI_DEFAULT_THEME_3__ */ 

    show_title_status_icon();
    draw_title();
    wgui_show_inputbox();
#if defined(__MMI_TOUCH_SCREEN__)
  	/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-11 18:5 */
  	#if 0
	   mmi_pen_editor_inline_full_screen_redraw_handwriting_area();
	#else
	wgui_show_virtual_keyboard();
	#endif
	/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-11 18:5 */	
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    show_softkey_background();
    show_left_softkey();
    show_right_softkey();
#if defined(__MMI_TOUCH_SCREEN__)
#if defined(__MMI_WGUI_CSK_ENABLE__)
     
    show_softkey(MMI_CENTER_SOFTKEY);
#endif /* defined(__MMI_WGUI_CSK_ENABLE__) */ 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
     
    if (RedrawSpellingOrCandidateBoxesFunction)
    {
        RedrawSpellingOrCandidateBoxesFunction();
    }
    /* CSD end */

}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_full_screen_edit_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_full_screen_edit_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 input_type = current_wgui_inline_text_edit_item->input_type & INPUT_TYPE_MASK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    
    ClearAllKeyHandler(); // clear all key to avoid inline edit screen use editor key handle
    gdi_layer_lock_frame_buffer();
 
    wgui_current_inline_item_type = 0;
    current_wgui_inline_item->flags &= ~INLINE_ITEM_OPEN;
    if (inline_full_screen_edit_changed)
    {
        wgui_inline_list_menu_changed = 1;
    }
    if (current_wgui_inline_item->flags & INLINE_ITEM_CHANGED)
    {
        set_wgui_inline_list_menu_changed();
    }
    current_wgui_inline_text_edit_item->UCS2_count = MMI_multiline_inputbox.UCS2_count;
    gui_strcpy(
        (UI_string_type) current_wgui_inline_text_edit_item->buffer,
        (UI_string_type) wgui_inline_full_screen_text_edit_buffer);
    if ((input_type == INPUT_TYPE_NUMERIC_PASSWORD) || (input_type == INPUT_TYPE_ALPHANUMERIC_PASSWORD) ||
        (input_type == INPUT_TYPE_SAT_NUMERIC_PASSWORD))
    {
        inline_edit_prepare_mask_buffer(
            current_wgui_inline_text_edit_item->buffer,
            current_wgui_inline_text_edit_item->mask_buffer);
        *current_wgui_inline_text_edit_text_p = current_wgui_inline_text_edit_item->mask_buffer;
    }
    else
    {
         
    #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        if ((IsInlineItemDefaultTextEnable()) &&
            (gui_strlen((UI_string_type) current_wgui_inline_text_edit_item->buffer) == 0))
        {
            *current_wgui_inline_text_edit_text_p = current_wgui_inline_text_edit_item->default_item_text;
        }
        else
    #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
             

            *current_wgui_inline_text_edit_text_p = current_wgui_inline_text_edit_item->buffer;

         
    #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        MMI_fixed_icontext_menuitems[wgui_inline_item_highlighted_index].item_text =
            (UI_string_type) (*current_wgui_inline_text_edit_text_p);
    #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
         
    }
    /* set_MMI_current_input_type(); */
    reset_softkeys();
    reset_multitaps();
    wgui_close_inputbox();

     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    if (inline_edit_scr_bg_ID != 0 || inline_edit_scr_bg_filename != NULL)       
    {
        dm_set_scr_bg_image(inline_edit_scr_bg_ID, inline_edit_scr_bg_filename, inline_edit_scr_bg_x, inline_edit_scr_bg_y, inline_edit_scr_bg_opacity);         
        inline_edit_scr_bg_ID = 0;
        inline_edit_scr_bg_filename = NULL;
    }
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
     

    // #ifdef __MMI_PLUTO_GPRS__
    inline_image_text_edit_flag = 0;
    // #endif

    register_inline_fixed_list_keys();
    inline_text_edit_RSK_function = NULL;
    clear_left_softkey();
    clear_right_softkey();
     
    set_right_softkey_function(UI_dummy_function, KEY_EVENT_LONG_PRESS);
    set_left_softkey_label(inline_full_screen_edit_saved_LSK_label);
    set_left_softkey_icon(inline_full_screen_edit_saved_LSK_icon);
    set_right_softkey_label(inline_full_screen_edit_saved_RSK_label);
    set_right_softkey_icon(inline_full_screen_edit_saved_RSK_icon);
    register_left_softkey_handler();
    register_right_softkey_handler();
    wgui_inline_item_handled = 0;
    wgui_text_menuitem_restart_scrolling();
    /* if title is changed, we should redraw title */
    if (MMI_title_icon != inline_full_screen_edit_saved_title_icon ||
        MMI_title_string != inline_full_screen_edit_saved_title)
    {
        MMI_title_icon = inline_full_screen_edit_saved_title_icon;
        MMI_title_string = inline_full_screen_edit_saved_title;
        draw_title();
    }
    current_wgui_inline_text_edit_item->edit_complete_callback(
                                            current_wgui_inline_text_edit_item->buffer,
                                            current_wgui_inline_text_edit_item->buffer_size);
    show_fixed_list();
     
    // draw_title();
     
    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    wgui_inputbox_UCS2_count_function = NULL;
    wgui_inputbox_not_empty_callback = NULL;
    wgui_inputbox_empty_callback = NULL;
    wgui_inputbox_validation_callback = NULL;
    inline_full_screen_edit_changed = 0;
    complete_inline_item_edit = UI_dummy_function;
#if defined(__MMI_TOUCH_SCREEN__)
#ifdef __MMI_WGUI_CSK_ENABLE__
     
    set_softkey_label(NULL, MMI_CENTER_SOFTKEY);
    set_softkey_icon(NULL, MMI_CENTER_SOFTKEY);
    redraw_softkey(MMI_CENTER_SOFTKEY);
    clear_softkey_handler(MMI_CENTER_SOFTKEY);
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 
    if (GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY != MMI_virtual_keyboard.lang_type)
    {
        resize_fixed_list(MMI_content_width, MMI_content_height - MMI_virtual_keyboard.height - 1);
        mmi_pen_editor_clear_and_show_virtual_keyboard_area();
    }
    else
    {
        resize_fixed_list(MMI_content_width, MMI_content_height);
    }
    redraw_fixed_list();
    redraw_current_inline_item();
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
//    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

    //gui_cancel_timer(UI_inputbox_blink_cursor);
     
    StopMyTimer(BLINKING_CURSOR);
    /* CSD end */
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_full_screen_edit_close
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_full_screen_edit_close(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    current_wgui_inline_item->flags |= INLINE_ITEM_OPEN;
    if (inline_full_screen_edit_changed)
    {
        current_wgui_inline_item->flags |= INLINE_ITEM_CHANGED;
    }
    gdi_layer_lock_frame_buffer();
    set_MMI_current_input_type();
    reset_softkeys();
    reset_multitaps();
    wgui_close_inputbox();

     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    if (inline_edit_scr_bg_ID != 0 || inline_edit_scr_bg_filename != NULL)       
    {
        dm_set_scr_bg_image(inline_edit_scr_bg_ID, inline_edit_scr_bg_filename, inline_edit_scr_bg_x, inline_edit_scr_bg_y, inline_edit_scr_bg_opacity);         
        inline_edit_scr_bg_ID = 0;
        inline_edit_scr_bg_filename = NULL;
    }
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
     

    wgui_inline_item_handled = 0;
     
    // it will cause the inline full(multiline input box parameters) screen's history not be stored
    //wgui_current_inline_item_type = 0;
    //CSD end
     
    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    wgui_inputbox_UCS2_count_function = NULL;
    wgui_inputbox_not_empty_callback = NULL;
    wgui_inputbox_empty_callback = NULL;
    wgui_inputbox_validation_callback = NULL;
    inline_full_screen_edit_changed = 0;
    complete_inline_item_edit = UI_dummy_function;
#if defined(__MMI_TOUCH_SCREEN__)
#ifdef __MMI_WGUI_CSK_ENABLE__
     
    if (GetActiveScreenId() != POPUP_SCREENID
        && GetActiveScreenId() != WAP_SCREEN_MESSAGE_NOTIFICATION
        //&& GetActiveScreenId() != SCR_ID_POC_CM
        && GetActiveScreenId() != SCR_ID_SAT_WAITSCREEN
        && GetActiveScreenId() != SCR_POPUP_SCREEN && GetActiveScreenId() != SCR_ID_MSG_POPUP)
    {
        set_softkey_label(NULL, MMI_CENTER_SOFTKEY);
        set_softkey_icon(NULL, MMI_CENTER_SOFTKEY);
        redraw_softkey(MMI_CENTER_SOFTKEY);
        clear_softkey_handler(MMI_CENTER_SOFTKEY);
    }
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
//    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

    //gui_cancel_timer(UI_inputbox_blink_cursor);
     
    StopMyTimer(BLINKING_CURSOR);
    /* CSD end */
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_full_screen_edit_cancel
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_full_screen_edit_cancel(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();//111406 inline cancel
    current_wgui_inline_item->flags &= ~INLINE_ITEM_OPEN;

    ClearAllKeyHandler(); // clear all key to avoid inline edit screen use editor key handle

    set_MMI_current_input_type();
    reset_softkeys();
    reset_multitaps();
    wgui_close_inputbox();

     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    if (inline_edit_scr_bg_ID != 0 || inline_edit_scr_bg_filename != NULL)       
    {
        dm_set_scr_bg_image(inline_edit_scr_bg_ID, inline_edit_scr_bg_filename, inline_edit_scr_bg_x, inline_edit_scr_bg_y, inline_edit_scr_bg_opacity);         
        inline_edit_scr_bg_ID = 0;
        inline_edit_scr_bg_filename = NULL;
    }
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
     

    // #ifdef __MMI_PLUTO_GPRS__
    inline_image_text_edit_flag = 0;
    // #endif

    register_inline_fixed_list_keys();
    inline_text_edit_RSK_function = NULL;

    clear_left_softkey();
    clear_right_softkey();
#if !defined(__MMI_WITH_C_KEY__)
    ClearKeyHandler(KEY_RSK, KEY_LONG_PRESS);
#endif 
    set_left_softkey_label(inline_full_screen_edit_saved_LSK_label);
    set_left_softkey_icon(inline_full_screen_edit_saved_LSK_icon);
    set_right_softkey_label(inline_full_screen_edit_saved_RSK_label);
    set_right_softkey_icon(inline_full_screen_edit_saved_RSK_icon);
    register_left_softkey_handler();
    register_right_softkey_handler();
    wgui_inline_item_handled = 0;
    wgui_current_inline_item_type = 0;
    wgui_text_menuitem_restart_scrolling();
    /* if title is changed, we should redraw title */
    if (MMI_title_icon != inline_full_screen_edit_saved_title_icon ||
        MMI_title_string != inline_full_screen_edit_saved_title)
    {
        MMI_title_icon = inline_full_screen_edit_saved_title_icon;
        MMI_title_string = inline_full_screen_edit_saved_title;
        draw_title();
    }
    show_fixed_list();
     
    // draw_title();
     
    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    wgui_inputbox_UCS2_count_function = NULL;
    wgui_inputbox_not_empty_callback = NULL;
    wgui_inputbox_empty_callback = NULL;
    wgui_inputbox_validation_callback = NULL;
    inline_full_screen_edit_changed = 0;
    complete_inline_item_edit = UI_dummy_function;

//    gdi_layer_unlock_frame_buffer();//111406 inline cancel
 //   gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

#if defined(__MMI_TOUCH_SCREEN__)
#ifdef __MMI_WGUI_CSK_ENABLE__
     
    set_softkey_label(NULL, MMI_CENTER_SOFTKEY);
    set_softkey_icon(NULL, MMI_CENTER_SOFTKEY);
    redraw_softkey(MMI_CENTER_SOFTKEY);
    clear_softkey_handler(MMI_CENTER_SOFTKEY);
#endif /* __MMI_WGUI_CSK_ENABLE__ */ 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

    //gui_cancel_timer(UI_inputbox_blink_cursor);
     
    StopMyTimer(BLINKING_CURSOR);
    /* CSD end */

}

extern void wgui_handle_inputbox_input(void);


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_edit_handle_multiline_inputbox_input
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_edit_handle_multiline_inputbox_input(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_handle_inputbox_input();
    inline_full_screen_edit_changed = 1;
#if defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__)
    if (wgui_current_inline_item_type == INLINE_ITEM_TYPE_MULTI_LINE_EDIT)
    {
    #ifdef __MMI_WITH_C_KEY__
        /* CSD for RSK display with C Key */
        /* Need to consider wgui_inline_list_menu_disable_done */
        if (wgui_inline_list_menu_changed && !(wgui_inline_list_menu_disable_done))
        {
            set_right_softkey_label(NULL);
            set_right_softkey_icon(NULL);
            redraw_right_softkey();
        }
    #else /* __MMI_WITH_C_KEY__ */ 
         
        if (wgui_inline_list_menu_changed && !(wgui_inline_list_menu_disable_done))
        {
            if (gui_multi_line_input_box_get_text_length(&MMI_multiline_inputbox) <= 0)
            {
                set_right_softkey_label(NULL);
                set_right_softkey_icon(NULL);
                redraw_right_softkey();
                if (MMI_right_softkey.flags & UI_BUTTON_STATE_CLICKED)
                {
                    MMI_right_softkey.flags &= ~UI_BUTTON_STATE_CLICKED;
                    MMI_right_softkey.flags &= ~UI_BUTTON_STATE_DOWN;
                }
            }
        }
    #endif /* __MMI_WITH_C_KEY__ */ 
    }
#endif /* defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__) */ 
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_full_screen_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_full_screen_edit(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 input_type;
    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) item;

    S32 inputbox_x = 0;
    S32 inputbox_y = (MMI_title_y + MMI_title_height);
    S32 inputbox_width = MMI_content_width;

#if defined(__MMI_FULL_SCREEN_EDITOR__)
    S32 inputbox_height = MMI_content_height;
#else 
    S32 inputbox_height = MMI_content_height - MMI_multitap_height;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_current_input_ext_type = 0;
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
    UI_UNUSED_PARAMETER(width);
    UI_UNUSED_PARAMETER(height);
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(flags);

    gdi_layer_lock_frame_buffer();
    wgui_text_menuitem_reset_scrolling();
    if (i->flags & INLINE_ITEM_CHANGED)
    {
        inline_full_screen_edit_changed = 1;
    }
    else
    {
        inline_full_screen_edit_changed = 0;
    }
    wgui_current_inline_item_type = INLINE_ITEM_TYPE_FULL_SCREEN_EDIT;
    current_wgui_inline_text_edit_item = i;
    current_wgui_inline_text_edit_text_p = text_p;
    input_type = i->input_type;

    if (!(i->flags & INLINE_ITEM_OPEN))
    {
        gui_strcpy((UI_string_type) wgui_inline_full_screen_text_edit_buffer, (UI_string_type) i->buffer);
        history_buffer = NULL;
    }
    i->flags &= ~INLINE_ITEM_OPEN;
    inline_full_screen_edit_saved_title = MMI_title_string;
    inline_full_screen_edit_saved_title_icon = MMI_title_icon;
    inline_full_screen_edit_saved_LSK_label = MMI_softkeys[MMI_LEFT_SOFTKEY].text;
    inline_full_screen_edit_saved_RSK_label = MMI_softkeys[MMI_RIGHT_SOFTKEY].text;
    inline_full_screen_edit_saved_LSK_icon = MMI_softkeys[MMI_LEFT_SOFTKEY].normal_up_icon;
    inline_full_screen_edit_saved_RSK_icon = MMI_softkeys[MMI_RIGHT_SOFTKEY].normal_up_icon;
    change_left_softkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    change_right_softkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    clear_key_handlers();
    change_left_softkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    change_right_softkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();
    MMI_title_string = get_string(i->title);
    MMI_title_icon = get_image(i->title_icon);
    register_hide_multitap(category5_hide_multitap);
    wgui_inputbox_validation_callback = i->validation_callback;

    if (i->custom_setup_function != NULL)
    {
        i->custom_setup_function();
    }
     
#ifdef __MMI_GB__
	GbInuptMethodEnterCategory5();
#elif defined __MMI_T9__
    InuptMethodEnterCategory5();
#elif defined __MMI_ZI__
    ZiInuptMethodEnterCategory5();
#elif defined __MMI_KA__
    KonkaInuptMethodEnterCategory5();
#elif defined __MMI_ITAP__
    ItapInuptMethodEnterCategory5();
#endif 
    /* CSD end */

    wgui_setup_inputbox(
        inputbox_x,
        inputbox_y,
        inputbox_width,
        inputbox_height,
        wgui_inline_full_screen_text_edit_buffer,
        i->buffer_size,
        INLINE_FULL_SCREEN_EDIT_HISTORY_ID,
        get_string(STR_GLOBAL_BACK),
        get_image(IMG_GLOBAL_BACK),
        (S16) input_type,
        history_buffer,
        1);
     
    MMI_current_input_ext_type = i->input_ext_type;
//fix bug 14870 20/10/2009
#if defined(__MMI_FULL_SCREEN_EDITOR__)
    inputbox_height = MMI_content_height - wgui_inputbox_information_bar_height;
#else 
    inputbox_height = MMI_content_height - MMI_multitap_height - wgui_inputbox_information_bar_height;
#endif 
    if (MMI_multiline_inputbox.height > inputbox_height)
    {
        gui_resize_multi_line_input_box(&MMI_multiline_inputbox, MMI_multiline_inputbox.width, inputbox_height);
    }

#if defined(__MMI_TOUCH_SCREEN__)
    mmi_pen_editor_resize_multiline_input_box_for_vk();
#endif 
    register_multiline_inputbox_input_callback(wgui_inline_edit_handle_multiline_inputbox_input);
    wgui_inputbox_RSK_function = handle_inline_full_screen_edit_cancel;
    redraw_current_inline_item = redraw_inline_full_screen_edit;
    /* set_left_softkey_function(handle_inline_full_screen_edit_complete,KEY_EVENT_UP); */

     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
    dm_get_scr_bg_image(
        &inline_edit_scr_bg_ID,
        &inline_edit_scr_bg_filename,
        &inline_edit_scr_bg_x,
        &inline_edit_scr_bg_y,
        &inline_edit_scr_bg_opacity);
    dm_set_scr_bg_image(inline_edit_scr_bg_ID, inline_edit_scr_bg_filename, inline_edit_scr_bg_x, inline_edit_scr_bg_y, MMI_EDITOR_BG_OPACITY);  
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
     

    gdi_layer_unlock_frame_buffer();
    redraw_inline_full_screen_edit();
    complete_inline_item_edit = handle_inline_full_screen_edit_close;

}

 


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemFullScreenEdit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item            [?]         
 *  title           [IN]        
 *  title_icon      [IN]        
 *  buffer          [?]         
 *  buffer_size     [IN]        
 *  input_type      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemFullScreenEdit(
        InlineItem *item,
        U16 title,
        U16 title_icon,
        U8 *buffer,
        S32 buffer_size,
        S16 input_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetInlineItemFullScreenEdit_ext(item, title, title_icon, buffer, buffer_size, input_type, 0);
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemFullScreenEdit_ext
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                    [?]         
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  buffer                  [?]         
 *  buffer_size             [IN]        
 *  input_type              [IN]        
 *  input_extended_type     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemFullScreenEdit_ext(
        InlineItem *item,
        U16 title,
        U16 title_icon,
        U8 *buffer,
        S32 buffer_size,
        S16 input_type,
        S16 input_extended_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i;
    UI_buffer_type mask_buffer;
    S16 t = input_type & INPUT_TYPE_MASK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_inline_text_edit_items >= MAX_INLINE_TEXT_EDIT_ITEMS)
    {
        return;
    }
    item->item = (void*)&wgui_inline_text_edit_items[n_inline_text_edit_items];
    item->flags = INLINE_ITEM_TYPE_FULL_SCREEN_EDIT;
    mask_buffer = inline_text_edit_mask_buffers[n_inline_text_edit_items];
    n_inline_text_edit_items++;
    i = (wgui_inline_item_text_edit*) item->item;
    i->max_lines = 0;
    i->title = title;
    i->title_icon = title_icon;
    i->buffer = buffer;
    i->buffer_size = buffer_size;
    i->flags = 0;
    i->input_type = input_type;
    i->input_ext_type = input_extended_type;
    item->text_p = i->buffer;
    item->handler = handle_inline_full_screen_edit;
    i->edit_complete_callback = UI_dummy_edit_complete_handler;
    i->custom_setup_function = NULL;
    i->validation_callback = UI_dummy_validation_function;
    if ((t == INPUT_TYPE_NUMERIC_PASSWORD) || (t == INPUT_TYPE_ALPHANUMERIC_PASSWORD) ||
        (t == INPUT_TYPE_SAT_NUMERIC_PASSWORD))
    {
        inline_edit_prepare_mask_buffer(buffer, mask_buffer);
        i->mask_buffer = mask_buffer;
        item->text_p = mask_buffer;
    }
    /* UCS2_count==-1 means this field hasn't been edited */
    i->UCS2_count = -1;
}


/*****************************************************************************
 * FUNCTION
 *  ReConfigureInlineItemFullScreenEdit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item            [?]         
 *  title           [IN]        
 *  title_icon      [IN]        
 *  buffer          [?]         
 *  buffer_size     [IN]        
 *  input_type      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ReConfigureInlineItemFullScreenEdit(
        InlineItem *item,
        U16 title,
        U16 title_icon,
        U8 *buffer,
        S32 buffer_size,
        S16 input_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i;
    UI_buffer_type mask_buffer;
    S16 t = input_type & INPUT_TYPE_MASK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((item->flags & 0xff) != INLINE_ITEM_TYPE_FULL_SCREEN_EDIT)
    {
        return;
    }
    i = (wgui_inline_item_text_edit*) item->item;
    mask_buffer = i->mask_buffer;
    i->max_lines = 0;
    i->title = title;
    i->title_icon = title_icon;
    i->buffer = buffer;
    i->buffer_size = buffer_size;
    i->flags = 0;
    i->input_type = input_type;
    item->text_p = i->buffer;
    i->edit_complete_callback = UI_dummy_edit_complete_handler;
    i->custom_setup_function = NULL;
    if ((t == INPUT_TYPE_NUMERIC_PASSWORD) || (t == INPUT_TYPE_ALPHANUMERIC_PASSWORD) ||
        (t == INPUT_TYPE_SAT_NUMERIC_PASSWORD))
    {
        inline_edit_prepare_mask_buffer(buffer, mask_buffer);
        i->mask_buffer = mask_buffer;
        item->text_p = mask_buffer;
    }
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineFullScreenEditCustomFunction
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineFullScreenEditCustomFunction(InlineItem *item, void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) item->item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i->custom_setup_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  RegisterInlineFullScreenEditCompleteHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterInlineFullScreenEditCompleteHandler(InlineItem *item, void (*f) (U8 *string, S32 buffer_size))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = (wgui_inline_item_text_edit*) item->item;
    if (f != NULL)
    {
        i->edit_complete_callback = f;
    }
}


/*****************************************************************************
 * FUNCTION
 *  RegisterInlineFullScreenEditValidationFunction
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterInlineFullScreenEditValidationFunction(
        InlineItem *item,
        void (*f) (U8 *buffer, U8 *cursor, S32 text_length))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = (wgui_inline_item_text_edit*) item->item;
    if (f != NULL)
    {
        i->validation_callback = f;
    }
}

/*----------------------------------------------------------------------------
   Text menu items used for inline select
----------------------------------------------------------------------------*/

#define  MAX_INLINE_SELECT_MENU_ITEMS  60

fixed_text_menuitem MMI_inline_select_item_common_data;
fixed_text_menuitem_type MMI_inline_select_items[MAX_INLINE_SELECT_MENU_ITEMS];
void *MMI_inline_select_items_p[MAX_INLINE_SELECT_MENU_ITEMS];
S32 *inline_select_menu_n_items;
horizontal_select MMI_inline_select_menu;

/* Temporary images used by Inline select menu  */

const U8 inline_select_left_arrow_image[] = 
{
    0x04, 0x01, 0x6F, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0C, 0x0E, 0x00, 0x00, 0x00,
    0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x00, 0x00, 0x00, 0x00, 0x34, 0x12, 0x34, 0x12,
    0x34, 0x12, 0x34, 0x12, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12,
    0x00, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x34, 0x12, 0x34, 0x12, 0x00, 0x00, 0xF8, 0x00,
    0xF8, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x00, 0x00, 0xF8, 0x00,
    0xF8, 0x00, 0x00, 0x00, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x00, 0x00, 0xF8, 0x00,
    0x00, 0x00, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x00, 0x00, 0x00, 0x00,
    0x34, 0x12,
};

const U8 inline_select_right_arrow_image[] = 
{
    0x04, 0x01, 0x6F, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0C, 0x0E, 0x00, 0x00, 0x00,
    0x34, 0x12, 0x00, 0x00, 0x00, 0x00, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12,
    0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x00, 0x00,
    0xF8, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x00, 0x00, 0xF8, 0x00,
    0xF8, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x34, 0x12, 0x34, 0x12, 0x00, 0x00, 0xF8, 0x00, 0xF8, 0x00,
    0x00, 0x00, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x34, 0x12,
    0x34, 0x12, 0x34, 0x12, 0x34, 0x12, 0x00, 0x00, 0x00, 0x00, 0x34, 0x12, 0x34, 0x12, 0x34, 0x12,
    0x34, 0x12,
};


/*****************************************************************************
 * FUNCTION
 *  wgui_show_inline_select_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  iwidth      [IN]        
 *  iheight     [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_inline_select_arrow(S32 x1, S32 y1, S32 iwidth, S32 iheight, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
    fixed_icontext_menuitem_type *t = (fixed_icontext_menuitem_type*) item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (t->flags & UI_MENUITEM_STATE_FOCUSSED)
    {
        draw_image_text_control(
            NULL,
            x1,
            y1,
            iwidth,
            iheight,
            item->side_img,
            (PU8) GetImage(IMG_H_SELECT_LEFT),
            (PU8) GetImage(IMG_H_SELECT_RIGHT),
            item->text_p,
            0);
    }
    else
    {
         
        /* show the background before showing the inline select item, in case of
           default text effect is enabled. */
    #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        if (IsInlineItemDefaultTextEnable())
        {
            inline_item_default_text_show_background((x1 - 1), y1, (x1 - 1) + (iwidth - 1), y1 + iheight - 1);
        }
    #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
         
        draw_image_text_control(NULL, x1, y1, iwidth, iheight, item->side_img, NULL, NULL, item->text_p, 0);
    }
     

}


/*****************************************************************************
 * FUNCTION
 *  initialize_inline_select_menu_items
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                       [IN]        
 *  y                       [IN]        
 *  width                   [IN]        
 *  height                  [IN]        
 *  n_items                 [IN]        
 *  list_of_items           [IN]        
 *  highlighted_item        [?]         
 *  f                       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void initialize_inline_select_menu_items(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 n_items,
        U8 **list_of_items,
        S32 *highlighted_item,
        void (*f) (S32 item_index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;
    UI_fixed_text_menuitem_theme *saved_themedata1 = current_fixed_text_menuitem_theme;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    current_fixed_text_menuitem_theme = &wgui_inline_select_fixed_text_menuitem_theme;
    gui_create_fixed_text_menuitem(&MMI_inline_select_item_common_data, width, height);
    gui_fixed_text_menuitem_set_text_position(&MMI_inline_select_item_common_data, 2, 0);
    MMI_inline_select_item_common_data.flags |= UI_MENUITEM_CENTER_TEXT_Y;
#if(UI_TEXT_MENUITEM_SCROLL_TYPE == UI_TEXT_MENUITEM_SCROLL_TYPE_MARQUEE)
    MMI_inline_select_item_common_data.flags |= UI_MENUITEM_MARQUEE_SCROLL;
#elif(UI_TEXT_MENUITEM_SCROLL_TYPE==UI_TEXT_MENUITEM_SCROLL_TYPE_TWO_DIRECTION)
    MMI_inline_select_item_common_data.flags |= UI_MENUITEM_TWO_DIRECTION_SCROLL;
#endif 
    wgui_text_menuitem_reset_scrolling = gui_fixed_text_menuitem_stop_scroll;
    wgui_text_menuitem_restart_scrolling = gui_fixed_text_menuitem_start_scroll;
    for (i = 0; i < n_items; i++)
    {
        MMI_inline_select_items[i].flags = UI_MENUITEM_CENTER_TEXT_X;
        MMI_inline_select_items[i].text = (UI_string_type) list_of_items[i];
        MMI_inline_select_items_p[i] = (void*)&MMI_inline_select_items[i];
    }
    temp_inline_select_menu_theme.background_filler = MMI_fixed_icontext_menuitem.focussed_filler;
    current_horizontal_select_theme = &temp_inline_select_menu_theme;
     
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    if (line_draw)
    {
        if (r2lMMIFlag)
            /* for gap of arrow icon from the grid */
        {
            gui_create_horizontal_select(&MMI_inline_select_menu, x + 1, y + 1, width - 5, height - 1);
        }
        else
        {
            gui_create_horizontal_select(&MMI_inline_select_menu, x + 1, y + 1, width - 2, height - 1);
        }
    }
    else
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
         
        gui_create_horizontal_select(&MMI_inline_select_menu, x, y, width, height);
    MMI_inline_select_menu.flags |= UI_LIST_MENU_LOOP;
    gui_set_horizontal_select_common_item_data(&MMI_inline_select_menu, (void*)&MMI_inline_select_item_common_data);
    gui_set_horizontal_select_item_functions(
        &MMI_inline_select_menu,
        gui_show_fixed_text_menuitem,
        gui_measure_fixed_text_menuitem,
        gui_highlight_fixed_text_menuitem,
        gui_remove_highlight_fixed_text_menuitem,
        UI_fixed_menuitem_dummy_hide_function);
    gui_set_horizontal_select_images(
        &MMI_inline_select_menu,
        (U8*) GetImage(IMG_H_SELECT_LEFT),
        (U8*) GetImage(IMG_H_SELECT_RIGHT));
    if (f != NULL)
    {
        MMI_inline_select_menu.item_highlighted = f;
    }
     
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    if (line_draw && r2lMMIFlag)
    {
        width = MMI_inline_select_menu.ix2 - MMI_inline_select_menu.ix1 - 4;
    }
    else
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
         
        width = MMI_inline_select_menu.ix2 - MMI_inline_select_menu.ix1;
    height = MMI_inline_select_menu.iy2 - MMI_inline_select_menu.iy1;
    gui_resize_fixed_text_menuitem(&MMI_inline_select_item_common_data, width, height);

    MMI_inline_select_menu.n_items = n_items;
    if (highlighted_item == NULL)
    {
        MMI_inline_select_menu.highlighted_item = 0;
    }
    else
    {
        MMI_inline_select_menu.highlighted_item = *highlighted_item;
    }

    if (MMI_inline_select_menu.highlighted_item > (MMI_inline_select_menu.n_items - 1))
    {
        MMI_inline_select_menu.highlighted_item = MMI_inline_select_menu.n_items - 1;
    }
    if (MMI_inline_select_menu.highlighted_item < 0)
    {
        MMI_inline_select_menu.highlighted_item = 0;
    }

    MMI_inline_select_menu.items = (void **)MMI_inline_select_items_p;
    MMI_inline_select_menu.item_highlight_function(
                            MMI_inline_select_menu. items[MMI_inline_select_menu.highlighted_item],
                            MMI_inline_select_menu.common_item_data);

    current_fixed_text_menuitem_theme = saved_themedata1;
}


/*****************************************************************************
 * FUNCTION
 *  show_inline_select_menu
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_inline_select_menu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x = 0, width = 0, height = 0, img_y1 = 0;
    PU8 image;
    UI_fixed_text_menuitem_theme *saved_themedata1 = current_fixed_text_menuitem_theme;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    current_fixed_text_menuitem_theme = &wgui_inline_select_fixed_text_menuitem_theme;
    gui_set_fixed_text_menuitem_current_theme(&MMI_inline_select_item_common_data);
    temp_inline_select_menu_theme.background_filler = MMI_fixed_icontext_menuitem.focussed_filler;
    current_horizontal_select_theme = &temp_inline_select_menu_theme;
    gui_set_horizontal_select_current_theme(&MMI_inline_select_menu);
     
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
     
    // This function call should be removed      
    // gui_inline_edit_disable_transparent_effect(&MMI_inline_select_menu);  // Disable for horizontel select
     
#endif /* __MMI_UI_TRANSPARENT_EFFECT__ */ 
     
     
    MMI_inline_select_item_common_data.text_font = &MMI_medium_font;
    gui_show_horizontal_select(&MMI_inline_select_menu);
    if (wgui_inline_items[wgui_inline_item_highlighted_index].show_img_flag == TRUE)
    {
        image = wgui_inline_items[wgui_inline_item_highlighted_index].side_img;
        if (image)
        {
            gui_measure_image(image, &width, &height);
            img_y1 = (MMI_INLINE_EDIT_MENUITEM_HEIGHT >> 1) - (height >> 1);
            if (r2lMMIFlag)
            {
                gui_show_transparent_image(
                    MMI_inline_select_menu.x + MMI_inline_select_menu.width + 2,
                    (MMI_inline_select_menu.y + img_y1),
                    image,
                    0);
            }
            else
            {
                gui_show_transparent_image(2, (MMI_inline_select_menu.y + img_y1), image, 0);
            }
            x = 2;
        }

    }
    else
    {
        x = MMI_inline_select_menu.x;
    }
    gdi_layer_blt_previous(
        MMI_inline_select_menu.x,
        MMI_inline_select_menu.y,
        MMI_inline_select_menu.x + MMI_inline_select_menu.width - 1,
        MMI_inline_select_menu.y + MMI_inline_select_menu.height - 1);
    current_fixed_text_menuitem_theme = saved_themedata1;
    UI_UNUSED_PARAMETER(x);
}


/*****************************************************************************
 * FUNCTION
 *  inline_select_menu_previous_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_select_menu_previous_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_horizontal_select_goto_previous_item(&MMI_inline_select_menu);
    show_inline_select_menu();
    set_wgui_inline_list_menu_changed();
}


/*****************************************************************************
 * FUNCTION
 *  inline_select_menu_next_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_select_menu_next_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_horizontal_select_goto_next_item(&MMI_inline_select_menu);
    show_inline_select_menu();
    set_wgui_inline_list_menu_changed();
}


/*****************************************************************************
 * FUNCTION
 *  initialize_inline_user_defined_select_menu_items
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x           [IN]        
 *  y           [IN]        
 *  width       [IN]        
 *  height      [IN]        
 *  text        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void initialize_inline_user_defined_select_menu_items(S32 x, S32 y, S32 width, S32 height, U8 *text)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_fixed_text_menuitem_theme *saved_themedata1 = current_fixed_text_menuitem_theme;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    current_fixed_text_menuitem_theme = &wgui_inline_select_fixed_text_menuitem_theme;
    gui_create_fixed_text_menuitem(&MMI_inline_select_item_common_data, width, height);
    gui_fixed_text_menuitem_set_text_position(&MMI_inline_select_item_common_data, 2, 0);
    MMI_inline_select_item_common_data.flags |= UI_MENUITEM_CENTER_TEXT_Y;
#if(UI_TEXT_MENUITEM_SCROLL_TYPE == UI_TEXT_MENUITEM_SCROLL_TYPE_MARQUEE)
    MMI_inline_select_item_common_data.flags |= UI_MENUITEM_MARQUEE_SCROLL;
#elif(UI_TEXT_MENUITEM_SCROLL_TYPE==UI_TEXT_MENUITEM_SCROLL_TYPE_TWO_DIRECTION)
    MMI_inline_select_item_common_data.flags |= UI_MENUITEM_TWO_DIRECTION_SCROLL;
#endif 
    wgui_text_menuitem_reset_scrolling = gui_fixed_text_menuitem_stop_scroll;
    wgui_text_menuitem_restart_scrolling = gui_fixed_text_menuitem_start_scroll;
    MMI_inline_select_items[0].flags = UI_MENUITEM_CENTER_TEXT_X;
    MMI_inline_select_items[0].text = (UI_string_type) text;
    MMI_inline_select_items_p[0] = (void*)&MMI_inline_select_items[0];
    temp_inline_select_menu_theme.background_filler = MMI_fixed_icontext_menuitem.focussed_filler;
    current_horizontal_select_theme = &temp_inline_select_menu_theme;
    gui_create_horizontal_select(&MMI_inline_select_menu, x, y, width, height);
    MMI_inline_select_menu.flags |= UI_LIST_MENU_LOOP;
    gui_set_horizontal_select_common_item_data(&MMI_inline_select_menu, (void*)&MMI_inline_select_item_common_data);
    gui_set_horizontal_select_item_functions(
        &MMI_inline_select_menu,
        gui_show_fixed_text_menuitem,
        gui_measure_fixed_text_menuitem,
        gui_highlight_fixed_text_menuitem,
        gui_remove_highlight_fixed_text_menuitem,
        UI_fixed_menuitem_dummy_hide_function);
    gui_set_horizontal_select_images(
        &MMI_inline_select_menu,
        (U8*) GetImage(IMG_H_SELECT_LEFT),
        (U8*) GetImage(IMG_H_SELECT_RIGHT));
    width = MMI_inline_select_menu.ix2 - MMI_inline_select_menu.ix1;
    height = MMI_inline_select_menu.iy2 - MMI_inline_select_menu.iy1;
    gui_resize_fixed_text_menuitem(&MMI_inline_select_item_common_data, width, height);

    MMI_inline_select_menu.n_items = 1;
    MMI_inline_select_menu.highlighted_item = 0;
    MMI_inline_select_menu.items = (void **)MMI_inline_select_items_p;
    MMI_inline_select_menu.item_highlight_function(
                            MMI_inline_select_menu. items[MMI_inline_select_menu.highlighted_item],
                            MMI_inline_select_menu.common_item_data);
    current_fixed_text_menuitem_theme = saved_themedata1;
}


/*****************************************************************************
 * FUNCTION
 *  show_inline_user_defined_select_menu
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_inline_user_defined_select_menu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_fixed_text_menuitem_theme *saved_themedata1 = current_fixed_text_menuitem_theme;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    current_fixed_text_menuitem_theme = &wgui_inline_select_fixed_text_menuitem_theme;
    gui_set_fixed_text_menuitem_current_theme(&MMI_inline_select_item_common_data);
    temp_inline_select_menu_theme.background_filler = MMI_fixed_icontext_menuitem.focussed_filler;
    current_horizontal_select_theme = &temp_inline_select_menu_theme;
    gui_set_horizontal_select_current_theme(&MMI_inline_select_menu);

    gui_show_horizontal_select(&MMI_inline_select_menu);
    gdi_layer_blt_previous(
        MMI_inline_select_menu.x,
        MMI_inline_select_menu.y,
        MMI_inline_select_menu.x + MMI_inline_select_menu.width - 1,
        MMI_inline_select_menu.y + MMI_inline_select_menu.height - 1);
    current_fixed_text_menuitem_theme = saved_themedata1;
}

/*----------------------------------------------------------------------------
   Inline select strings implementation
----------------------------------------------------------------------------*/

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
#define MAX_INLINE_SELECT_ITEMS        25       /* Made changes by Ritesh */
#else 
#define MAX_INLINE_SELECT_ITEMS        10
#endif 
extern S32 UI_text_menuitem_scroll_time;     

S32 n_inline_select_items = 0;
wgui_inline_item_select wgui_inline_select_items[MAX_INLINE_SELECT_ITEMS];
wgui_inline_item_select *current_wgui_inline_select_item = NULL;
S32 *current_wgui_inline_select_highlighted_item = NULL;
U8 **current_wgui_inline_select_text_p = NULL;


/*****************************************************************************
 * FUNCTION
 *  clear_inline_select_menu_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_inline_select_menu_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    clear_keyboard_key_handler();
    clear_inline_item_keys = UI_dummy_function;
     
    UI_text_menuitem_scroll_time = UI_TEXT_MENUITEM_SCROLL_TIME;
}


/*****************************************************************************
 * FUNCTION
 *  inline_select_menu_change_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_select_menu_change_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_text_menuitem_reset_scrolling();
    wgui_text_menuitem_reset_scrolling = gui_fixed_icontext_menuitem_stop_scroll;
    wgui_text_menuitem_restart_scrolling = gui_fixed_icontext_menuitem_start_scroll;
    clear_inline_select_menu_keys();
    *(current_wgui_inline_select_item->highlighted_item) = MMI_inline_select_menu.highlighted_item;
    *current_wgui_inline_select_text_p =
        current_wgui_inline_select_item->list_of_strings[*(current_wgui_inline_select_item->highlighted_item)];
    MMI_fixed_icontext_menuitems[wgui_inline_item_highlighted_index].item_text =
        (UI_string_type) current_wgui_inline_select_item->
        list_of_strings[*(current_wgui_inline_select_item->highlighted_item)];
    *current_wgui_inline_select_highlighted_item = MMI_inline_select_menu.highlighted_item;
    redraw_current_inline_item = UI_dummy_function;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = UI_dummy_inline_item_pen_function;
#endif 
    current_wgui_inline_select_highlighted_item = NULL;
    wgui_inline_item_handled = 0;
     
    MMI_inline_select_menu.left_arrow_image = NULL;
    MMI_inline_select_menu.right_arrow_image = NULL;
     
    complete_inline_item_edit = UI_dummy_function;
}


/*****************************************************************************
 * FUNCTION
 *  inline_select_menu_handle_up_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_select_menu_handle_up_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_select_menu_change_complete();
    inline_fixed_list_goto_previous_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_select_menu_handle_down_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_select_menu_handle_down_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_select_menu_change_complete();
    inline_fixed_list_goto_next_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_select_menu_keyboard_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  vkey_code       [IN]        
 *  key_state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_select_menu_keyboard_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 37:
                inline_select_menu_previous_item();
                break;
            case 38:
                inline_select_menu_handle_up_arrow();
                break;
            case 39:
                inline_select_menu_next_item();
                break;
            case 40:
                inline_select_menu_handle_down_arrow();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}


/*****************************************************************************
 * FUNCTION
 *  register_inline_select_menu_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_inline_select_menu_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(inline_select_menu_previous_item, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_select_menu_next_item, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_select_menu_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_select_menu_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_select_menu_handle_up_arrow, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(inline_select_menu_handle_down_arrow, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    register_keyboard_key_handler(inline_select_menu_keyboard_key_handler);
}


/*****************************************************************************
 * FUNCTION
 *  redraw_inline_select
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_inline_select(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    show_inline_select_menu();
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  inline_select_pen_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pen_event       [IN]        
 *  x               [IN]        
 *  y               [IN]        
 *  item_event      [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL inline_select_pen_handler(mmi_pen_event_type_enum pen_event, S16 x, S16 y, gui_inline_item_pen_enum *item_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret;
    gui_horizontal_select_pen_enum select_event;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ret = gui_horizontal_select_translate_pen_event(&MMI_inline_select_menu, pen_event, x, y, &select_event);

    if (ret)
    {
        switch (select_event)
        {
            case GUI_HORIZONTAL_SELECT_PEN_NONE:
                /* Do nothing */
                break;
            case GUI_HORIZONTAL_SELECT_PEN_PREV:
                inline_select_menu_previous_item();
                break;
            case GUI_HORIZONTAL_SELECT_PEN_NEXT:
                inline_select_menu_next_item();
                break;
            default:
                MMI_ASSERT(0);
        }
    }

    *item_event = GUI_INLINE_ITEM_PEN_NONE;
    return ret;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  current_inline_select_highlight_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void current_inline_select_highlight_handler(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *current_wgui_inline_select_highlighted_item = index;
    current_wgui_inline_select_item->item_highlighted(index);
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_select
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_select(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_select *i = (wgui_inline_item_select*) item;
    S32 xx;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(flags);

     
    UI_text_menuitem_scroll_time = 300;
     
    if (!r2lMMIFlag)
    {
        if (MMI_fixed_icontext_menuitem.text_x == 2)
        {
            xx = -1;
        }
        else
        {
            xx = MMI_fixed_icontext_menuitem.text_x;
        }
    }
    else
    {
        xx = 1;
    }
    /* CSD end */
    if (flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    clear_inline_item_keys = clear_inline_select_menu_keys;
    current_wgui_inline_select_item = i;
    current_wgui_inline_select_text_p = text_p;
     
    if (!r2lMMIFlag)
    {
        initialize_inline_select_menu_items(
            x + xx,
            y,
            width - xx,
            height,
            i->n_items,
            i->list_of_strings,
            i->highlighted_item,
            current_inline_select_highlight_handler);
    }
    else
    {
        if (flags & INLINE_ITEM_LEFT_ALIGN)
        {
            initialize_inline_select_menu_items(
                x + xx,
                y,
                width - 1 - xx,
                height,
                i->n_items,
                i->list_of_strings,
                i->highlighted_item,
                current_inline_select_highlight_handler);
        }
        else
        {
            initialize_inline_select_menu_items(
                x + xx,
                y,
                width - 1 - MMI_fixed_icontext_menuitem.text_x,
                height,
                i->n_items,
                i->list_of_strings,
                i->highlighted_item,
                current_inline_select_highlight_handler);
        }
    }

    /* CSD end */
    register_inline_select_menu_keys();

    if (current_wgui_inline_select_highlighted_item == NULL)
    {
        current_wgui_inline_select_highlighted_item = i->highlighted_item;
        current_inline_select_highlight_handler(*i->highlighted_item);
    }
    else
    {
        current_wgui_inline_select_highlighted_item = i->highlighted_item;
    }
    show_inline_select_menu();
    complete_inline_item_edit = inline_select_menu_change_complete;
    redraw_current_inline_item = redraw_inline_select;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = inline_select_pen_handler;
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemSelect
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                    [?]         
 *  n_items                 [IN]        
 *  list_of_items           [IN]        
 *  highlighted_item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemSelect(InlineItem *item, S32 n_items, U8 **list_of_items, S32 *highlighted_item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_select *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_inline_select_items >= MAX_INLINE_SELECT_ITEMS)
    {
        return;
    }
    item->item = (void*)&wgui_inline_select_items[n_inline_select_items];
    item->flags = INLINE_ITEM_TYPE_SELECT;
    n_inline_select_items++;
    i = (wgui_inline_item_select*) item->item;
    i->flags = 0;
    i->list_of_strings = list_of_items;
    i->n_items = n_items;
    i->item_highlighted = UI_dummy_function_S32 ;
    if (highlighted_item == NULL)
    {
        i->highlighted_item = NULL;
        item->text_p = list_of_items[0];
    }
    else
    {
        if (*highlighted_item < 0 || *highlighted_item >= n_items)
        {
            MMI_DBG_ASSERT(0);
            *highlighted_item = 0;
        }
        i->highlighted_item = highlighted_item;
        item->text_p = list_of_items[*highlighted_item];
    }
    item->handler = handle_inline_select;
}


/*****************************************************************************
 * FUNCTION
 *  ReConfigureInlineItemSelect
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                    [?]         
 *  n_items                 [IN]        
 *  list_of_items           [IN]        
 *  highlighted_item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ReConfigureInlineItemSelect(InlineItem *item, S32 n_items, U8 **list_of_items, S32 *highlighted_item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_select *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((item->flags & 0xff) != INLINE_ITEM_TYPE_SELECT)
    {
        return;
    }
    i = (wgui_inline_item_select*) item->item;
    i->flags = 0;
    i->list_of_strings = list_of_items;
    i->n_items = n_items;
    i->item_highlighted = UI_dummy_function_S32 ;
    if (highlighted_item == NULL)
    {
        i->highlighted_item = NULL;
        item->text_p = list_of_items[0];
    }
    else
    {
        if (*highlighted_item < 0 || *highlighted_item >= n_items)
        {
            MMI_DBG_ASSERT(0);
            *highlighted_item = 0;
        }
        i->highlighted_item = highlighted_item;
        item->text_p = list_of_items[*highlighted_item];
    }
}


/*****************************************************************************
 * FUNCTION
 *  RegisterInlineSelectHighlightHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterInlineSelectHighlightHandler(InlineItem *item, void (*f) (S32 item_index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_select *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = (wgui_inline_item_select*) item->item;
    if (f != NULL)
    {
        i->item_highlighted = f;
    }
}


/*****************************************************************************
 * FUNCTION
 *  GetInlineSelectHighlightedItem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 GetInlineSelectHighlightedItem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (*current_wgui_inline_select_item->highlighted_item);
}

/*----------------------------------------------------------------------------
   Inline date edit implementation
----------------------------------------------------------------------------*/

#define MAX_INLINE_DATE_ITEMS                   4
#define MAX_INLINE_DATE_DISPLAY_STRING_SIZE     26   /* 12 chars with null terminator */

S32 n_inline_date_items = 0;
wgui_inline_item_date wgui_inline_date_items[MAX_INLINE_DATE_ITEMS];
U8 wgui_inline_date_display_strings[MAX_INLINE_DATE_ITEMS][MAX_INLINE_DATE_DISPLAY_STRING_SIZE];
wgui_inline_item_date *current_wgui_inline_date_item;
U8 **current_wgui_inline_date_text_p;
U8 wgui_inline_date_edit_day_buffer[3 * ENCODING_LENGTH];
U8 wgui_inline_date_edit_month_buffer[3 * ENCODING_LENGTH];
U8 wgui_inline_date_edit_year_buffer[5 * ENCODING_LENGTH];
date_input wgui_inline_date_input;


/*****************************************************************************
 * FUNCTION
 *  handle_inline_date_edit_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_date_edit_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_inline_fixed_list_keys();
    gdi_layer_lock_frame_buffer();
    gui_strcpy(
        (UI_string_type) current_wgui_inline_date_item->day_buffer,
        (UI_string_type) wgui_inline_date_edit_day_buffer);
    gui_strcpy(
        (UI_string_type) current_wgui_inline_date_item->month_buffer,
        (UI_string_type) wgui_inline_date_edit_month_buffer);
    gui_strcpy(
        (UI_string_type) current_wgui_inline_date_item->year_buffer,
        (UI_string_type) wgui_inline_date_edit_year_buffer);
    current_wgui_inline_date_item->string_construct_callback(
                                    *current_wgui_inline_date_text_p,
                                    current_wgui_inline_date_item->day_buffer,
                                    current_wgui_inline_date_item->month_buffer,
                                    current_wgui_inline_date_item->year_buffer);
    MMI_fixed_icontext_menuitems[wgui_inline_item_highlighted_index].item_text =
        (UI_string_type) * current_wgui_inline_date_text_p;
    clear_left_softkey();
    clear_right_softkey();
#if(0)
	set_left_softkey_label(inline_full_screen_edit_saved_LSK_label);
	set_left_softkey_icon(inline_full_screen_edit_saved_LSK_icon);
	set_right_softkey_label(inline_full_screen_edit_saved_RSK_label);
	set_right_softkey_icon(inline_full_screen_edit_saved_RSK_icon);
#endif /* (0) */ 
    register_left_softkey_handler();
    register_right_softkey_handler();
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_close_input_box();
#endif 
    wgui_inline_item_handled = 0;
    wgui_text_menuitem_restart_scrolling();
    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    complete_inline_item_edit = UI_dummy_function;
    set_current_date_input(NULL);
    clear_date_input_callback();
#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
     
    //But in in-line editor, redrawing is needed when completing in-line item
#ifdef __MMI_WGUI_CSK_ENABLE__
    redraw_softkey(MMI_CENTER_SOFTKEY);
#endif 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
//    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_date_edit_cancel
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_date_edit_cancel(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_inline_fixed_list_keys();
    gdi_layer_lock_frame_buffer();
    clear_left_softkey();
    clear_right_softkey();
#if(0)
	set_left_softkey_label(inline_full_screen_edit_saved_LSK_label);
	set_left_softkey_icon(inline_full_screen_edit_saved_LSK_icon);
	set_right_softkey_label(inline_full_screen_edit_saved_RSK_label);
	set_right_softkey_icon(inline_full_screen_edit_saved_RSK_icon);
#endif /* (0) */ 
    register_left_softkey_handler();
    register_right_softkey_handler();
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_close_input_box();
#endif 
    wgui_inline_item_handled = 0;
    wgui_text_menuitem_restart_scrolling();
    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    complete_inline_item_edit = UI_dummy_function;
    set_current_date_input(NULL);
    clear_date_input_callback();
#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
     
    //But in in-line editor, redrawing is needed when completing in-line item
#ifdef __MMI_WGUI_CSK_ENABLE__
    redraw_softkey(MMI_CENTER_SOFTKEY);
#endif 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
 //   gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  redraw_inline_date_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_inline_date_edit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    date_input_show_background(&wgui_inline_date_input);
    show_date_input(&wgui_inline_date_input);
}


/*****************************************************************************
 * FUNCTION
 *  inline_date_edit_handle_down_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_date_edit_handle_down_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    handle_inline_date_edit_complete();
    inline_fixed_list_goto_next_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_date_edit_handle_up_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_date_edit_handle_up_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    handle_inline_date_edit_complete();
    inline_fixed_list_goto_previous_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_date_input_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  vkey_code       [IN]        
 *  key_state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_date_input_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 37:
                date_input_previous_character();
                break;
            case 38:
                inline_date_edit_handle_up_arrow();
                break;
            case 39:
                date_input_next_character();
                break;
            case 40:
                inline_date_edit_handle_down_arrow();
                break;
            case 36:    /* home */
                break;
            case 35:    /* end */
                break;
            case 33:    /* page up */
                break;
            case 34:    /* page down */
                break;
            case 45:
                date_input_toggle_insert_mode();
                break;
            case 46:
                date_input_delete_current_character();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_date_edit_input
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_date_edit_input(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_wgui_inline_list_menu_changed();
}

/* History implementation for inline date edit  */

typedef struct _inline_date_edit_history
{
    U16 history_ID;
    S16 current_focus;
    S16 current_position;
    S16 dummy;  /* Added for history issue on hardware */
    U32 flags;
} inline_date_edit_history;


/*****************************************************************************
 * FUNCTION
 *  inline_date_edit_get_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 *inline_date_edit_get_history(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        U16 hID = (U16) (INLINE_DATE_HISTORY_ID | 0x8000);
        inline_date_edit_history *h = (inline_date_edit_history*) history_buffer;

        h->history_ID = hID;
        h->current_focus = (S16) wgui_inline_date_input.current_focus;
        if (wgui_inline_date_input.focus_input_box != NULL)
        {
            h->current_position =
                (S16) (wgui_inline_date_input.focus_input_box->current_text_p -
                       wgui_inline_date_input.focus_input_box->text);
        }
        h->flags = wgui_inline_date_input.flags;
    }
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  inline_date_edit_set_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  
 *****************************************************************************/
U8 inline_date_edit_set_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        inline_date_edit_history *h = (inline_date_edit_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            wgui_inline_date_input.current_focus = h->current_focus;
            date_input_reset_focus(&wgui_inline_date_input);
            date_input_set_focus(&wgui_inline_date_input);
            if (wgui_inline_date_input.focus_input_box != NULL)
            {
                wgui_inline_date_input.focus_input_box->current_text_p =
                    wgui_inline_date_input.focus_input_box->text + h->current_position;
            }
            wgui_inline_date_input.flags = h->flags;
            return (1);
        }
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_date_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_date_edit(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_date *i = (wgui_inline_item_date*) item;
    S32 xx;
    U32 dtflags = DATE_INPUT_TYPE_YYYY_MM_DD;

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_handwriting_area_struct stroke_area;
    mmi_pen_handwriting_area_struct ext_stroke;
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(flags);
    wgui_current_inline_item_type = INLINE_ITEM_TYPE_DATE;
     
    if (!r2lMMIFlag)
    {
        if (MMI_fixed_icontext_menuitem.text_x == 2)
        {
            xx = -1;
        }
        else
        {
            xx = MMI_fixed_icontext_menuitem.text_x;
        }
    }
    else
    {
        xx = 1;
    }
    /* CSD end */
    if (flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    switch (flags & INLINE_ITEM_JUSTIFY_MASK)
    {
        case INLINE_ITEM_RIGHT_JUSTIFY:
            dtflags |= DATE_INPUT_RIGHT_JUSTIFY;
            break;
        case INLINE_ITEM_CENTER_JUSTIFY:
            dtflags |= DATE_INPUT_CENTER_JUSTIFY;
            break;
    }
    complete_inline_item_edit = handle_inline_date_edit_complete;
    current_wgui_inline_date_item = i;
    current_wgui_inline_date_text_p = text_p;
    gui_strcpy((UI_string_type) wgui_inline_date_edit_day_buffer, (UI_string_type) i->day_buffer);
    gui_strcpy((UI_string_type) wgui_inline_date_edit_month_buffer, (UI_string_type) i->month_buffer);
    gui_strcpy((UI_string_type) wgui_inline_date_edit_year_buffer, (UI_string_type) i->year_buffer);
     
    if (!r2lMMIFlag)
    {
        create_date_input(
            &wgui_inline_date_input,
            x + xx,
            y,
            width - 1 - xx,
            height,
            dtflags,
            wgui_inline_date_edit_day_buffer,
            wgui_inline_date_edit_month_buffer,
            wgui_inline_date_edit_year_buffer);
    }
    else
    {
         
    #ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
        /* To reduce the highlighter's width */
        if (flags & INLINE_ITEM_LEFT_ALIGN)
        {
            create_date_input(
                &wgui_inline_date_input,
                x + xx,
                y,
                width - 1 - xx,
                height,
                dtflags,
                wgui_inline_date_edit_day_buffer,
                wgui_inline_date_edit_month_buffer,
                wgui_inline_date_edit_year_buffer);
        }
        else
        {
            create_date_input(
                &wgui_inline_date_input,
                x + xx,
                y,
                width - 1 - MMI_fixed_icontext_menuitem.text_x - 4,
                height,
                dtflags,
                wgui_inline_date_edit_day_buffer,
                wgui_inline_date_edit_month_buffer,
                wgui_inline_date_edit_year_buffer);
        }
    #else /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
        if (flags & INLINE_ITEM_LEFT_ALIGN)
        {
            create_date_input(
                &wgui_inline_date_input,
                x + xx,
                y,
                width - 1 - xx,
                height,
                dtflags,
                wgui_inline_date_edit_day_buffer,
                wgui_inline_date_edit_month_buffer,
                wgui_inline_date_edit_year_buffer);
        }
        else
        {
            create_date_input(
                &wgui_inline_date_input,
                x + xx,
                y,
                width - 1 - MMI_fixed_icontext_menuitem.text_x,
                height,
                dtflags,
                wgui_inline_date_edit_day_buffer,
                wgui_inline_date_edit_month_buffer,
                wgui_inline_date_edit_year_buffer);
        }
    #endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
         
    }
    /* CSD end */
    inline_date_edit_set_history(INLINE_DATE_HISTORY_ID, history_buffer);
    date_input_set_seperator(&wgui_inline_date_input, (UI_character_type) '/');
    date_input_show_background(&wgui_inline_date_input);
    show_date_input(&wgui_inline_date_input);
    set_current_date_input(&wgui_inline_date_input);
    date_input_register_keys();
    register_keyboard_key_handler(inline_date_input_key_handler);
    SetKeyHandler(inline_date_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_date_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_date_edit_handle_down_arrow, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    SetKeyHandler(inline_date_edit_handle_up_arrow, KEY_VOL_UP, KEY_EVENT_DOWN);
    redraw_current_inline_item = redraw_inline_date_edit;
    register_date_input_callback(handle_inline_date_edit_input);
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    ext_stroke.x1 = 0;
    ext_stroke.y1 = 0;
    ext_stroke.x2 = UI_device_width - 1;
    ext_stroke.y2 = UI_device_height - 1;
#if 0
#else /* 0 */ 
    stroke_area.x1 = MMI_content_x;
    stroke_area.x2 = MMI_content_x + MMI_content_width - MMI_fixed_list_menu.vbar.width;
    stroke_area.y1 = MMI_title_y;
    stroke_area.y2 = MMI_title_y + MMI_content_height + MMI_title_height;
#endif /* 0 */ 
    mmi_pen_editor_setup_input_box(&stroke_area, &ext_stroke, INPUT_TYPE_NUMERIC, MMI_FALSE, PEN_EDITOR_DATE);
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemDate
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                [?]         
 *  day_buffer          [?]         
 *  month_buffer        [?]         
 *  year_buffer         [?]         
 *  f                   [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemDate(
        InlineItem *item,
        U8 *day_buffer,
        U8 *month_buffer,
        U8 *year_buffer,
        void (*f) (U8 *string_buffer, U8 *day_buffer, U8 *month_buffer, U8 *year_buffer))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_date *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_inline_date_items >= MAX_INLINE_DATE_ITEMS)
    {
        return;
    }
    item->item = (void*)&wgui_inline_date_items[n_inline_date_items];
    item->flags = INLINE_ITEM_TYPE_DATE;
    item->text_p = (U8*) wgui_inline_date_display_strings[n_inline_date_items];
    n_inline_date_items++;
    i = (wgui_inline_item_date*) item->item;
    i->day_buffer = day_buffer;
    i->month_buffer = month_buffer;
    i->year_buffer = year_buffer;
    i->flags = INLINE_ITEM_LEFT_JUSTIFY;
    i->string_construct_callback = f;
    i->string_construct_callback(item->text_p, i->day_buffer, i->month_buffer, i->year_buffer);
    item->handler = handle_inline_date_edit;
}


/*****************************************************************************
 * FUNCTION
 *  ReConfigureInlineItemDate
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                [?]         
 *  day_buffer          [?]         
 *  month_buffer        [?]         
 *  year_buffer         [?]         
 *  f                   [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ReConfigureInlineItemDate(
        InlineItem *item,
        U8 *day_buffer,
        U8 *month_buffer,
        U8 *year_buffer,
        void (*f) (U8 *string_buffer, U8 *day_buffer, U8 *month_buffer, U8 *year_buffer))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_date *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((item->flags & 0xff) != INLINE_ITEM_TYPE_DATE)
    {
        return;
    }
    i = (wgui_inline_item_date*) item->item;
    i->day_buffer = day_buffer;
    i->month_buffer = month_buffer;
    i->year_buffer = year_buffer;
    i->flags = 0;
    i->string_construct_callback = f;
    i->string_construct_callback(item->text_p, i->day_buffer, i->month_buffer, i->year_buffer);
}

/* Displaying date edit as a menu item in the fixed list menu  */

date_input wgui_inline_date_input_display;
U8 wgui_inline_date_edit_day_display_buffer[3 * ENCODING_LENGTH];
U8 wgui_inline_date_edit_month_display_buffer[3 * ENCODING_LENGTH];
U8 wgui_inline_date_edit_year_display_buffer[5 * ENCODING_LENGTH];


/*****************************************************************************
 * FUNCTION
 *  wgui_set_date_time_input_focussed_theme
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_date_time_input_focussed_theme(void)
{
#ifndef __MMI_MAINLCD_220X176__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifndef __MMI_MAINLCD_220X176__
    color_t c1 = {0, 0, 0, 100};
#endif
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    date_time_input_theme.disabled_filler = wgui_inline_singleline_inputbox_focussed_theme.disabled_filler;
    date_time_input_theme.normal_filler = wgui_inline_singleline_inputbox_focussed_theme.normal_filler;
    date_time_input_theme.selected_filler = wgui_inline_singleline_inputbox_focussed_theme.selected_filler;
    date_time_input_theme.disabled_text_color = wgui_inline_singleline_inputbox_focussed_theme.disabled_text_color;
    date_time_input_theme.normal_text_color = wgui_inline_singleline_inputbox_focussed_theme.normal_text_color;
    date_time_input_theme.selected_text_color = wgui_inline_singleline_inputbox_focussed_theme.selected_text_color;
    date_time_input_background_filler = *wgui_inline_singleline_inputbox_focussed_theme.normal_filler;
    date_time_input_background_filler.flags &= 0x00000fff;
    date_time_input_background_filler.flags |= UI_FILLED_AREA_SINGLE_BORDER;

#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    date_time_input_background_filler.border_color = current_MMI_theme->list_background_filler->border_color;
#else 
#ifdef __MMI_MAINLCD_220X176__
    date_time_input_background_filler.border_color = current_MMI_theme->list_background_filler->border_color;
#else
    date_time_input_background_filler.border_color = c1;
#endif 
#endif 
     
    date_time_input_theme.cursor_color = *current_MMI_theme->inline_edit_cursor_color;
    date_time_input_theme.selection_text_color = *current_MMI_theme->inline_edit_selected_text_color;
    date_time_input_theme.selection_color = *current_MMI_theme->inline_edit_selector_color;
#endif
}


/*****************************************************************************
 * FUNCTION
 *  wgui_set_date_time_input_unfocussed_theme
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_date_time_input_unfocussed_theme(void)
{
#ifndef __MMI_MAINLCD_220X176__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c1 = {0, 0, 0, 100};

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    date_time_input_theme.disabled_filler = wgui_inline_singleline_inputbox_unfocussed_theme.disabled_filler;
     
    /* changed the date time normal filler in case of default text effect is enabled. */
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        date_time_input_theme.normal_filler = &temp_inline_edit_normal_filler;
    }
    else
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
         
        date_time_input_theme.normal_filler = wgui_inline_singleline_inputbox_unfocussed_theme.normal_filler;
    date_time_input_theme.selected_filler = wgui_inline_singleline_inputbox_unfocussed_theme.selected_filler;
    date_time_input_theme.disabled_text_color = wgui_inline_singleline_inputbox_unfocussed_theme.disabled_text_color;
    date_time_input_theme.normal_text_color = wgui_inline_singleline_inputbox_unfocussed_theme.normal_text_color;
    date_time_input_theme.selected_text_color = wgui_inline_singleline_inputbox_unfocussed_theme.selected_text_color;
    date_time_input_background_filler = *wgui_inline_singleline_inputbox_unfocussed_theme.normal_filler;
    date_time_input_background_filler.flags &= 0x00000fff;
    date_time_input_background_filler.flags |= UI_FILLED_AREA_SINGLE_BORDER;
     
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    date_time_input_background_filler.border_color = current_MMI_theme->list_background_filler->border_color;
#else 
    date_time_input_background_filler.border_color = c1;
#endif 
     
    time_period_input_background_filler = MMI_inputbox_normal_filler;   /* *MMI_fixed_list_menu.normal_filler; */

    date_time_input_theme.cursor_color = *current_MMI_theme->inline_edit_cursor_color;
    date_time_input_theme.selection_text_color = *current_MMI_theme->inline_edit_selected_text_color;
    date_time_input_theme.selection_color = *current_MMI_theme->inline_edit_selector_color;
#endif
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_inline_date_list_menuitem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  width       [IN]        
 *  height      [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_inline_date_list_menuitem(S32 x1, S32 y1, S32 width, S32 height, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_date *i = (wgui_inline_item_date*) item->item;
    S32 xx;
    U32 dtflags = DATE_INPUT_TYPE_YYYY_MM_DD;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    if (MMI_fixed_icontext_menuitem.text_x == 2)
    {
        xx = -1;
    }
    else
    {
        xx = MMI_fixed_icontext_menuitem.text_x;
    }
    if (item->flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    switch (item->flags & INLINE_ITEM_JUSTIFY_MASK)
    {
        case INLINE_ITEM_RIGHT_JUSTIFY:
            dtflags |= DATE_INPUT_RIGHT_JUSTIFY;
            break;
        case INLINE_ITEM_CENTER_JUSTIFY:
            dtflags |= DATE_INPUT_CENTER_JUSTIFY;
            break;
    }
    gui_strcpy((UI_string_type) wgui_inline_date_edit_day_display_buffer, (UI_string_type) i->day_buffer);
    gui_strcpy((UI_string_type) wgui_inline_date_edit_month_display_buffer, (UI_string_type) i->month_buffer);
    gui_strcpy((UI_string_type) wgui_inline_date_edit_year_display_buffer, (UI_string_type) i->year_buffer);
    wgui_set_date_time_input_unfocussed_theme();
    create_date_input(
        &wgui_inline_date_input_display,
        x1 + xx,
        y1,
        width - 1 - xx,
        height,
        dtflags,
        wgui_inline_date_edit_day_display_buffer,
        wgui_inline_date_edit_month_display_buffer,
        wgui_inline_date_edit_year_display_buffer);
    date_input_set_seperator(&wgui_inline_date_input_display, (UI_character_type) '/');
     
    /* show the different background in case of default text effect is enabled
       for the inline item type Date. */
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        inline_item_default_text_show_background(
            wgui_inline_date_input_display.x,
            wgui_inline_date_input_display.y,
            wgui_inline_date_input_display.x + wgui_inline_date_input_display.width - 1,
            wgui_inline_date_input_display.y + wgui_inline_date_input_display.height - 1);
    }
    else
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
         
        date_input_show_background(&wgui_inline_date_input_display);
    date_input_reset_focus(&wgui_inline_date_input_display);
    show_date_input(&wgui_inline_date_input_display);
    wgui_set_date_time_input_focussed_theme();
    redraw_left_softkey();
    redraw_right_softkey();
    gdi_layer_unlock_frame_buffer();
}

/*----------------------------------------------------------------------------
   Inline time edit implementation
----------------------------------------------------------------------------*/

#define MAX_INLINE_TIME_ITEMS                   4
#define MAX_INLINE_TIME_DISPLAY_STRING_SIZE           24

S32 n_inline_time_items = 0;
wgui_inline_item_time wgui_inline_time_items[MAX_INLINE_TIME_ITEMS];
U8 wgui_inline_time_display_strings[MAX_INLINE_TIME_ITEMS][MAX_INLINE_TIME_DISPLAY_STRING_SIZE];
wgui_inline_item_time *current_wgui_inline_time_item;
U8 **current_wgui_inline_time_text_p;
U8 wgui_inline_time_edit_hours_buffer[3 * ENCODING_LENGTH];
U8 wgui_inline_time_edit_minutes_buffer[3 * ENCODING_LENGTH];
U8 wgui_inline_time_edit_seconds_buffer[3 * ENCODING_LENGTH];
U8 *wgui_inline_time_AM_PM_flag;
time_input wgui_inline_time_input;


/*****************************************************************************
 * FUNCTION
 *  handle_inline_time_edit_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_time_edit_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_inline_fixed_list_keys();
    gdi_layer_lock_frame_buffer();
    gui_strcpy(
        (UI_string_type) current_wgui_inline_time_item->hours_buffer,
        (UI_string_type) wgui_inline_time_edit_hours_buffer);
    gui_strcpy(
        (UI_string_type) current_wgui_inline_time_item->minutes_buffer,
        (UI_string_type) wgui_inline_time_edit_minutes_buffer);
    current_wgui_inline_time_item->AM_PM_flag = wgui_inline_time_AM_PM_flag;
    current_wgui_inline_time_item->string_construct_callback(
                                    *current_wgui_inline_time_text_p,
                                    current_wgui_inline_time_item->hours_buffer,
                                    current_wgui_inline_time_item->minutes_buffer,
                                    current_wgui_inline_time_item->AM_PM_flag);
    MMI_fixed_icontext_menuitems[wgui_inline_item_highlighted_index].item_text =
        (UI_string_type) * current_wgui_inline_time_text_p;
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_close_input_box();
#endif 
    wgui_inline_item_handled = 0;
    wgui_text_menuitem_restart_scrolling();
    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    complete_inline_item_edit = UI_dummy_function;
    set_current_time_input(NULL);
    clear_time_input_callback();
#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
     
    //But in in-line editor, redrawing is needed when completing in-line item
#ifdef __MMI_WGUI_CSK_ENABLE__
    redraw_softkey(MMI_CENTER_SOFTKEY);
#endif 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
   // gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_time_edit_cancel
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_time_edit_cancel(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_inline_fixed_list_keys();
    gdi_layer_lock_frame_buffer();
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_close_input_box();
#endif 
    wgui_inline_item_handled = 0;
    wgui_text_menuitem_restart_scrolling();
    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    complete_inline_item_edit = UI_dummy_function;
    set_current_time_input(NULL);
    clear_time_input_callback();
#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
     
    //But in in-line editor, redrawing is needed when completing in-line item
#ifdef __MMI_WGUI_CSK_ENABLE__
    redraw_softkey(MMI_CENTER_SOFTKEY);
#endif 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
    //gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  redraw_inline_time_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_inline_time_edit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    time_input_show_background(&wgui_inline_time_input);
    show_time_input(&wgui_inline_time_input);
}


/*****************************************************************************
 * FUNCTION
 *  inline_time_edit_handle_down_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_time_edit_handle_down_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    handle_inline_time_edit_complete();
    inline_fixed_list_goto_next_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_time_edit_handle_up_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_time_edit_handle_up_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    handle_inline_time_edit_complete();
    inline_fixed_list_goto_previous_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_time_input_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  vkey_code       [IN]        
 *  key_state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_time_input_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 37:
                time_input_previous_character();
                break;
            case 38:
                inline_time_edit_handle_up_arrow();
                break;
            case 39:
                time_input_next_character();
                break;
            case 40:
                inline_time_edit_handle_down_arrow();
                break;
            case 36:    /* home */
                break;
            case 35:    /* end */
                break;
            case 33:    /* page up */
                break;
            case 34:    /* page down */
                break;
            case 45:
                time_input_toggle_insert_mode();
                break;
            case 46:
                time_input_delete_current_character();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_time_edit_input
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_time_edit_input(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_wgui_inline_list_menu_changed();
}

/* History implementation for inline time edit  */

typedef struct _inline_time_edit_history
{
    U16 history_ID;
    S16 current_focus;
    S16 current_position;
    S16 dummy;  /* Added for history issue on hardware */
    U32 flags;
} inline_time_edit_history;


/*****************************************************************************
 * FUNCTION
 *  inline_time_edit_get_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 *inline_time_edit_get_history(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        U16 hID = (U16) (INLINE_TIME_HISTORY_ID | 0x8000);
        inline_time_edit_history *h = (inline_time_edit_history*) history_buffer;

        h->history_ID = hID;
        h->current_focus = (S16) wgui_inline_time_input.current_focus;
        if (wgui_inline_time_input.focus_input_box != NULL)
        {
            h->current_position =
                (S16) (wgui_inline_time_input.focus_input_box->current_text_p -
                       wgui_inline_time_input.focus_input_box->text);
        }
        h->flags = wgui_inline_time_input.flags;
    }
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  inline_time_edit_set_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  
 *****************************************************************************/
U8 inline_time_edit_set_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        inline_time_edit_history *h = (inline_time_edit_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            wgui_inline_time_input.current_focus = h->current_focus;
            time_input_reset_focus(&wgui_inline_time_input);
            time_input_set_focus(&wgui_inline_time_input);
            if (wgui_inline_time_input.focus_input_box != NULL)
            {
                wgui_inline_time_input.focus_input_box->current_text_p =
                    wgui_inline_time_input.focus_input_box->text + h->current_position;
            }
            wgui_inline_time_input.flags = h->flags;
            return (1);
        }
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_time_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_time_edit(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_time *i = (wgui_inline_item_time*) item;
    S32 xx;
    U32 dtflags = TIME_INPUT_TYPE_HH_MM;

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_handwriting_area_struct stroke_area;
    mmi_pen_handwriting_area_struct ext_stroke;
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(flags);
    wgui_current_inline_item_type = INLINE_ITEM_TYPE_TIME;
     
    if (!r2lMMIFlag)
    {
        if (MMI_fixed_icontext_menuitem.text_x == 2)
        {
            xx = -1;
        }
        else
        {
            xx = MMI_fixed_icontext_menuitem.text_x;
        }
    }
    else
    {
        xx = 1;
    }

    /* CSD end */
    if (flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    switch (flags & INLINE_ITEM_JUSTIFY_MASK)
    {
        case INLINE_ITEM_RIGHT_JUSTIFY:
            dtflags |= TIME_INPUT_RIGHT_JUSTIFY;
            break;
        case INLINE_ITEM_CENTER_JUSTIFY:
            dtflags |= TIME_INPUT_CENTER_JUSTIFY;
            break;
    }
    complete_inline_item_edit = handle_inline_time_edit_complete;
    current_wgui_inline_time_item = i;
    current_wgui_inline_time_text_p = text_p;
    gui_strcpy((UI_string_type) wgui_inline_time_edit_hours_buffer, (UI_string_type) i->hours_buffer);
    gui_strcpy((UI_string_type) wgui_inline_time_edit_minutes_buffer, (UI_string_type) i->minutes_buffer);
    wgui_inline_time_AM_PM_flag = i->AM_PM_flag;
     
    if (!r2lMMIFlag)
    {
        create_time_input(
            &wgui_inline_time_input,
            x + xx,
            y,
            width - 1 - xx,
            height,
            dtflags,
            wgui_inline_time_edit_hours_buffer,
            wgui_inline_time_edit_minutes_buffer,
            wgui_inline_time_edit_seconds_buffer);
    }
    else
    {
        if (flags & INLINE_ITEM_LEFT_ALIGN)
        {
            create_time_input(
                &wgui_inline_time_input,
                x + xx,
                y,
                width - 1 - xx,
                height,
                dtflags,
                wgui_inline_time_edit_hours_buffer,
                wgui_inline_time_edit_minutes_buffer,
                wgui_inline_time_edit_seconds_buffer);
        }
        else
        {
            create_time_input(
                &wgui_inline_time_input,
                x + xx,
                y,
                width - 1 - MMI_fixed_icontext_menuitem.text_x,
                height,
                dtflags,
                wgui_inline_time_edit_hours_buffer,
                wgui_inline_time_edit_minutes_buffer,
                wgui_inline_time_edit_seconds_buffer);
        }
    }
    /* CSD end */
    inline_time_edit_set_history(INLINE_TIME_HISTORY_ID, history_buffer);
    time_input_set_seperator(&wgui_inline_time_input, (UI_character_type) ':');
    time_input_show_background(&wgui_inline_time_input);
    show_time_input(&wgui_inline_time_input);
    set_current_time_input(&wgui_inline_time_input);
    time_input_register_keys();
    register_keyboard_key_handler(inline_time_input_key_handler);
    SetKeyHandler(inline_time_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_time_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_time_edit_handle_down_arrow, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    SetKeyHandler(inline_time_edit_handle_up_arrow, KEY_VOL_UP, KEY_EVENT_DOWN);
    register_time_input_callback(handle_inline_time_edit_input);
    redraw_current_inline_item = redraw_inline_time_edit;
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    ext_stroke.x1 = 0;
    ext_stroke.y1 = 0;
    ext_stroke.x2 = UI_device_width - 1;
    ext_stroke.y2 = UI_device_height - 1;
#if 0
#else /* 0 */ 
    stroke_area.x1 = MMI_content_x;
    stroke_area.x2 = MMI_content_x + MMI_content_width - MMI_fixed_list_menu.vbar.width;
    stroke_area.y1 = MMI_title_y;
    stroke_area.y2 = MMI_title_y + MMI_content_height + MMI_title_height;
#endif /* 0 */ 
    mmi_pen_editor_setup_input_box(&stroke_area, &ext_stroke, INPUT_TYPE_NUMERIC, MMI_FALSE, PEN_EDITOR_TIME);
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemTime
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                [?]         
 *  hours_buffer        [?]         
 *  minutes_buffer      [?]         
 *  AM_PM_flag          [?]         
 *  f                   [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemTime(
        InlineItem *item,
        U8 *hours_buffer,
        U8 *minutes_buffer,
        U8 *AM_PM_flag,
        void (*f) (U8 *string_buffer, U8 *hours_buffer, U8 *minutes_buffer, U8 *AM_PM_flag))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_time *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_inline_time_items >= MAX_INLINE_TIME_ITEMS)
    {
        return;
    }
    item->item = (void*)&wgui_inline_time_items[n_inline_time_items];
    item->flags = INLINE_ITEM_TYPE_TIME;
    item->text_p = (U8*) wgui_inline_time_display_strings[n_inline_time_items];
    n_inline_time_items++;
    i = (wgui_inline_item_time*) item->item;
    i->hours_buffer = hours_buffer;
    i->minutes_buffer = minutes_buffer;
    i->seconds_buffer = NULL;
    i->AM_PM_flag = AM_PM_flag;
    i->flags = INLINE_ITEM_LEFT_JUSTIFY;
    i->string_construct_callback = f;
    i->string_construct_callback(item->text_p, i->hours_buffer, i->minutes_buffer, i->AM_PM_flag);
    item->handler = handle_inline_time_edit;
}


/*****************************************************************************
 * FUNCTION
 *  ReConfigureInlineItemTime
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                [?]         
 *  hours_buffer        [?]         
 *  minutes_buffer      [?]         
 *  AM_PM_flag          [?]         
 *  f                   [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ReConfigureInlineItemTime(
        InlineItem *item,
        U8 *hours_buffer,
        U8 *minutes_buffer,
        U8 *AM_PM_flag,
        void (*f) (U8 *string_buffer, U8 *hours_buffer, U8 *minutes_buffer, U8 *AM_PM_flag))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_time *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((item->flags & 0xff) != INLINE_ITEM_TYPE_TIME)
    {
        return;
    }
    i = (wgui_inline_item_time*) item->item;
    i->hours_buffer = hours_buffer;
    i->minutes_buffer = minutes_buffer;
    i->AM_PM_flag = AM_PM_flag;
    i->flags = 0;
    i->string_construct_callback = f;
    i->string_construct_callback(item->text_p, i->hours_buffer, i->minutes_buffer, i->AM_PM_flag);
}

/* Displaying time edit as a menu item in the fixed list menu  */

U8 wgui_inline_time_edit_hours_display_buffer[3 * ENCODING_LENGTH];
U8 wgui_inline_time_edit_minutes_display_buffer[3 * ENCODING_LENGTH];
U8 wgui_inline_time_edit_seconds_display_buffer[3 * ENCODING_LENGTH];
U8 *wgui_inline_time_AM_PM_display_flag;
time_input wgui_inline_time_input_display;


/*****************************************************************************
 * FUNCTION
 *  wgui_show_inline_time_list_menuitem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  width       [IN]        
 *  height      [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_inline_time_list_menuitem(S32 x1, S32 y1, S32 width, S32 height, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_time *i = (wgui_inline_item_time*) item->item;
    S32 xx;
    U32 dtflags = TIME_INPUT_TYPE_HH_MM;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    if (MMI_fixed_icontext_menuitem.text_x == 2)
    {
        xx = -1;
    }
    else
    {
        xx = MMI_fixed_icontext_menuitem.text_x;
    }
    if (item->flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    switch (item->flags & INLINE_ITEM_JUSTIFY_MASK)
    {
        case INLINE_ITEM_RIGHT_JUSTIFY:
            dtflags |= DATE_INPUT_RIGHT_JUSTIFY;
            break;
        case INLINE_ITEM_CENTER_JUSTIFY:
            dtflags |= DATE_INPUT_CENTER_JUSTIFY;
            break;
    }
    gui_strcpy((UI_string_type) wgui_inline_time_edit_hours_display_buffer, (UI_string_type) i->hours_buffer);
    gui_strcpy((UI_string_type) wgui_inline_time_edit_minutes_display_buffer, (UI_string_type) i->minutes_buffer);
    gui_strcpy((UI_string_type) wgui_inline_time_edit_seconds_display_buffer, (UI_string_type) i->seconds_buffer);
    wgui_inline_time_AM_PM_display_flag = i->AM_PM_flag;
    wgui_set_date_time_input_unfocussed_theme();
    create_time_input(
        &wgui_inline_time_input_display,
        x1 + xx,
        y1,
        width - 1 - xx,
        height,
        dtflags,
        wgui_inline_time_edit_hours_display_buffer,
        wgui_inline_time_edit_minutes_display_buffer,
        wgui_inline_time_edit_seconds_display_buffer);
    time_input_set_seperator(&wgui_inline_time_input_display, (UI_character_type) ':');
     
    /* show the different background in case of default text effect is enabled
       for the inline item type Time. */
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        inline_item_default_text_show_background(
            wgui_inline_time_input_display.x,
            wgui_inline_time_input_display.y,
            wgui_inline_time_input_display.x + wgui_inline_time_input_display.width - 1,
            wgui_inline_time_input_display.y + wgui_inline_time_input_display.height - 1);
    }
    else
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
         
        time_input_show_background(&wgui_inline_time_input_display);
    time_input_reset_focus(&wgui_inline_time_input_display);
    show_time_input(&wgui_inline_time_input_display);
    wgui_set_date_time_input_focussed_theme();
    redraw_left_softkey();
    redraw_right_softkey();
    gdi_layer_unlock_frame_buffer();
}

/*----------------------------------------------------------------------------
   Inline user defined select strings implementation
----------------------------------------------------------------------------*/

#define MAX_INLINE_USER_DEFINED_SELECT_ITEMS       4

S32 n_inline_user_defined_select_items = 0;
wgui_inline_item_user_defined_select wgui_inline_user_defined_select_items[MAX_INLINE_USER_DEFINED_SELECT_ITEMS];
wgui_inline_item_user_defined_select *current_wgui_inline_user_defined_select_item;
U8 **current_wgui_inline_user_defined_select_text_p;


/*****************************************************************************
 * FUNCTION
 *  inline_user_defined_select_menu_previous_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_user_defined_select_menu_previous_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    current_wgui_inline_user_defined_select_item->current_text_p =
        current_wgui_inline_user_defined_select_item->previous_item_callback();
    *current_wgui_inline_user_defined_select_text_p = current_wgui_inline_user_defined_select_item->current_text_p;
    MMI_inline_select_items[0].text = (UI_string_type) * current_wgui_inline_user_defined_select_text_p;
    show_inline_user_defined_select_menu();
    set_wgui_inline_list_menu_changed();
}


/*****************************************************************************
 * FUNCTION
 *  inline_user_defined_select_menu_next_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_user_defined_select_menu_next_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    current_wgui_inline_user_defined_select_item->current_text_p =
        current_wgui_inline_user_defined_select_item->next_item_callback();
    *current_wgui_inline_user_defined_select_text_p = current_wgui_inline_user_defined_select_item->current_text_p;
    MMI_inline_select_items[0].text = (UI_string_type) * current_wgui_inline_user_defined_select_text_p;
    show_inline_user_defined_select_menu();
    set_wgui_inline_list_menu_changed();
}


/*****************************************************************************
 * FUNCTION
 *  inline_user_defined_select_menu_change_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_user_defined_select_menu_change_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_text_menuitem_reset_scrolling();
    wgui_text_menuitem_reset_scrolling = gui_fixed_icontext_menuitem_stop_scroll;
    wgui_text_menuitem_restart_scrolling = gui_fixed_icontext_menuitem_start_scroll;
    clear_inline_select_menu_keys();
    MMI_fixed_icontext_menuitems[wgui_inline_item_highlighted_index].item_text =
        (UI_string_type) * current_wgui_inline_user_defined_select_text_p;
    redraw_current_inline_item = UI_dummy_function;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = UI_dummy_inline_item_pen_function;
#endif 
    complete_inline_item_edit = UI_dummy_function;
    wgui_inline_item_handled = 0;
}


/*****************************************************************************
 * FUNCTION
 *  inline_user_defined_select_menu_handle_up_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_user_defined_select_menu_handle_up_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_user_defined_select_menu_change_complete();
    inline_fixed_list_goto_previous_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_user_defined_select_menu_handle_down_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_user_defined_select_menu_handle_down_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_user_defined_select_menu_change_complete();
    inline_fixed_list_goto_next_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_user_defined_select_menu_keyboard_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  vkey_code       [IN]        
 *  key_state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_user_defined_select_menu_keyboard_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 37:
                inline_user_defined_select_menu_previous_item();
                break;
            case 38:
                inline_user_defined_select_menu_handle_up_arrow();
                break;
            case 39:
                inline_user_defined_select_menu_next_item();
                break;
            case 40:
                inline_user_defined_select_menu_handle_down_arrow();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}


/*****************************************************************************
 * FUNCTION
 *  register_inline_user_defined_select_menu_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_inline_user_defined_select_menu_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(inline_user_defined_select_menu_previous_item, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_user_defined_select_menu_next_item, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_user_defined_select_menu_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_user_defined_select_menu_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_user_defined_select_menu_handle_up_arrow, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(inline_user_defined_select_menu_handle_down_arrow, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    register_keyboard_key_handler(inline_user_defined_select_menu_keyboard_key_handler);
}


/*****************************************************************************
 * FUNCTION
 *  redraw_inline_user_defined_select
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_inline_user_defined_select(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    show_inline_user_defined_select_menu();
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  inline_user_defined_select_pen_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pen_event       [IN]        
 *  x               [IN]        
 *  y               [IN]        
 *  item_event      [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL inline_user_defined_select_pen_handler(
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_inline_item_pen_enum *item_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret;
    gui_horizontal_select_pen_enum select_event;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ret = gui_horizontal_select_translate_pen_event(&MMI_inline_select_menu, pen_event, x, y, &select_event);

    if (ret)
    {
        switch (select_event)
        {
            case GUI_HORIZONTAL_SELECT_PEN_NONE:
                /* Do nothing */
                break;
            case GUI_HORIZONTAL_SELECT_PEN_PREV:
                inline_user_defined_select_menu_previous_item();
                break;
            case GUI_HORIZONTAL_SELECT_PEN_NEXT:
                inline_user_defined_select_menu_next_item();
                break;
            default:
                MMI_ASSERT(0);
        }
    }

    *item_event = GUI_INLINE_ITEM_PEN_NONE;
    return ret;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  handle_inline_user_defined_select
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_user_defined_select(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_user_defined_select *i = (wgui_inline_item_user_defined_select*) item;
    S32 xx;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(flags);
    if (MMI_fixed_icontext_menuitem.text_x == 2)
    {
        xx = -1;
    }
    else
    {
        xx = MMI_fixed_icontext_menuitem.text_x;
    }
    if (flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    clear_inline_item_keys = clear_inline_select_menu_keys;
    current_wgui_inline_user_defined_select_item = i;
    current_wgui_inline_user_defined_select_text_p = text_p;
    initialize_inline_user_defined_select_menu_items(x + xx, y, width - xx, height, i->current_text_p);
    show_inline_user_defined_select_menu();
    register_inline_user_defined_select_menu_keys();
    redraw_current_inline_item = redraw_inline_user_defined_select;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = inline_user_defined_select_pen_handler;
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  dummy_inline_item_user_defined_select_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
U8 *dummy_inline_item_user_defined_select_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (NULL);
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemUserDefinedSelect
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                        [?]         
 *  current_item_callback       [IN]        
 *  previous_item_callback      [IN]        
 *  next_item_callback          [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemUserDefinedSelect(
        InlineItem *item,
        U8 *(*current_item_callback) (void),
        U8 *(*previous_item_callback) (void),
        U8 *(*next_item_callback) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_user_defined_select *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_inline_user_defined_select_items >= MAX_INLINE_USER_DEFINED_SELECT_ITEMS)
    {
        return;
    }
    item->item = (void*)&wgui_inline_user_defined_select_items[n_inline_user_defined_select_items];
    item->flags = INLINE_ITEM_TYPE_USER_DEFINED_SELECT;
    n_inline_user_defined_select_items++;
    i = (wgui_inline_item_user_defined_select*) item->item;
    i->flags = 0;
    if (current_item_callback == NULL)
    {
        i->current_item_callback = dummy_inline_item_user_defined_select_callback;
    }
    else
    {
        i->current_item_callback = current_item_callback;
    }
    if (previous_item_callback == NULL)
    {
        i->previous_item_callback = dummy_inline_item_user_defined_select_callback;
    }
    else
    {
        i->previous_item_callback = previous_item_callback;
    }
    if (next_item_callback == NULL)
    {
        i->next_item_callback = dummy_inline_item_user_defined_select_callback;
    }
    else
    {
        i->next_item_callback = next_item_callback;
    }
    i->current_text_p = current_item_callback();
    item->text_p = i->current_text_p;
    item->handler = handle_inline_user_defined_select;
}


/*****************************************************************************
 * FUNCTION
 *  ReConfigureInlineItemUserDefinedSelect
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                        [?]         
 *  current_item_callback       [IN]        
 *  previous_item_callback      [IN]        
 *  next_item_callback          [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ReConfigureInlineItemUserDefinedSelect(
        InlineItem *item,
        U8 *(*current_item_callback) (void),
        U8 *(*previous_item_callback) (void),
        U8 *(*next_item_callback) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_user_defined_select *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((item->flags & 0xff) != INLINE_ITEM_TYPE_USER_DEFINED_SELECT)
    {
        return;
    }
    i = (wgui_inline_item_user_defined_select*) item->item;
    i->flags = 0;
    if (current_item_callback == NULL)
    {
        i->current_item_callback = dummy_inline_item_user_defined_select_callback;
    }
    else
    {
        i->current_item_callback = current_item_callback;
    }
    if (previous_item_callback == NULL)
    {
        i->previous_item_callback = dummy_inline_item_user_defined_select_callback;
    }
    else
    {
        i->previous_item_callback = previous_item_callback;
    }
    if (next_item_callback == NULL)
    {
        i->next_item_callback = dummy_inline_item_user_defined_select_callback;
    }
    else
    {
        i->next_item_callback = next_item_callback;
    }
    i->current_text_p = current_item_callback();
    item->text_p = i->current_text_p;
}

/*----------------------------------------------------------------------------
   Inline select Day Of Week implementation
----------------------------------------------------------------------------*/

#define MAX_INLINE_DOW_SELECT_ITEMS       2

S32 n_inline_DOW_items = 0;
wgui_inline_item_DOW_select wgui_inline_DOW_select_items[MAX_INLINE_DOW_SELECT_ITEMS];
wgui_inline_item_DOW_select *current_wgui_inline_DOW_item = NULL;
U8 *current_wgui_inline_DOW_states = NULL;
U8 **current_wgui_inline_DOW_text_p = NULL;
DOW_select MMI_inline_DOW_select;
S32 current_DOW_select_highlighted_index = -1;


/*****************************************************************************
 * FUNCTION
 *  clear_inline_DOW_select_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_inline_DOW_select_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    clear_keyboard_key_handler();
    clear_inline_item_keys = UI_dummy_function;
}


/*****************************************************************************
 * FUNCTION
 *  inline_DOW_select_change_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_DOW_select_change_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    clear_inline_DOW_select_keys();
    *current_wgui_inline_DOW_text_p = (U8*) "";
    MMI_fixed_icontext_menuitems[wgui_inline_item_highlighted_index].item_text = (UI_string_type) "";
    for (i = 0; i < 7; i++)
    {
        current_wgui_inline_DOW_states[i] = MMI_inline_DOW_select.states[i];
    }
    redraw_current_inline_item = UI_dummy_function;
    complete_inline_item_edit = UI_dummy_function;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = UI_dummy_inline_item_pen_function;
#endif 
    clear_left_softkey();
    clear_right_softkey();
    wgui_inline_item_handled = 0;
}


/*****************************************************************************
 * FUNCTION
 *  inline_DOW_select_handle_up_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_DOW_select_handle_up_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_DOW_select_change_complete();
    inline_fixed_list_goto_previous_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_DOW_select_handle_down_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_DOW_select_handle_down_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_DOW_select_change_complete();
    inline_fixed_list_goto_next_item();
}


/*****************************************************************************
 * FUNCTION
 *  show_inline_DOW_select
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_inline_DOW_select(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    /* don't call the function to show background from here, call it from the
       function, which is calling this function, in case of default text effect
       is enabled. */
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (!(IsInlineItemDefaultTextEnable()))
#endif 
         
        show_DOW_select_background(&MMI_inline_DOW_select);
    show_DOW_select(&MMI_inline_DOW_select);
     
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    if (line_draw)
         
    {
        gui_draw_horizontal_line(
            MMI_inline_DOW_select.x,
            MMI_inline_DOW_select.x + MMI_inline_DOW_select.width - 1,
            MMI_inline_DOW_select.y + MMI_inline_DOW_select.height - 1,
            current_MMI_theme->list_background_filler->border_color);
        gui_draw_vertical_line(
            MMI_inline_DOW_select.y,
            MMI_inline_DOW_select.y + MMI_inline_DOW_select.height - 1,
            MMI_inline_DOW_select.x + MMI_inline_DOW_select.width - 1 - 2,
            current_MMI_theme->list_background_filler->border_color);
        gui_draw_vertical_line(
            MMI_inline_DOW_select.y,
            MMI_inline_DOW_select.y + MMI_inline_DOW_select.height - 1,
            MMI_fixed_list_menu.vbar.x,
            current_MMI_theme->list_background_filler->border_color);
    }
     
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
     
     
    gdi_layer_blt_previous(
        MMI_inline_DOW_select.x,
        MMI_inline_DOW_select.y,
        MMI_inline_DOW_select.x + MMI_inline_DOW_select.width - 1,
        MMI_inline_DOW_select.y + MMI_inline_DOW_select.height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  set_inline_DOW_select_list_menu_theme
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_inline_DOW_select_list_menu_theme(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_DOW_select_list_menu_theme(&MMI_inline_DOW_select);
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_DOW_select_LSK_display
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_DOW_select_LSK_display(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_inline_DOW_select.states[MMI_inline_DOW_select.highlighted_cell])
    {
        set_left_softkey_label(get_string(WGUI_CATEGORY_OFF_STRING_ID));
    }
    else
    {
        set_left_softkey_label(get_string(WGUI_CATEGORY_ON_STRING_ID));
    }
    redraw_left_softkey();
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_DOW_select_LSK_up
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_DOW_select_LSK_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DOW_select_toggle_item(&MMI_inline_DOW_select);
     
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        show_DOW_select_background(&MMI_inline_DOW_select);
    }
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
     
    show_inline_DOW_select();
    handle_inline_DOW_select_LSK_display();
    set_wgui_inline_list_menu_changed();
}


/*****************************************************************************
 * FUNCTION
 *  inline_DOW_select_previous_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_DOW_select_previous_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DOW_select_previous(&MMI_inline_DOW_select);
    current_DOW_select_highlighted_index = MMI_inline_DOW_select.highlighted_cell;
     
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        show_DOW_select_background(&MMI_inline_DOW_select);
    }
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
     
    show_inline_DOW_select();
    handle_inline_DOW_select_LSK_display();
}


/*****************************************************************************
 * FUNCTION
 *  inline_DOW_select_next_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_DOW_select_next_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DOW_select_next(&MMI_inline_DOW_select);
    current_DOW_select_highlighted_index = MMI_inline_DOW_select.highlighted_cell;
     
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        show_DOW_select_background(&MMI_inline_DOW_select);
    }
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
     
    show_inline_DOW_select();
    handle_inline_DOW_select_LSK_display();
}


/*****************************************************************************
 * FUNCTION
 *  inline_DOW_select_keyboard_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  vkey_code       [IN]        
 *  key_state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_DOW_select_keyboard_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 37:
                inline_DOW_select_previous_item();
                break;
            case 38:
                inline_DOW_select_handle_up_arrow();
                break;
            case 39:
                inline_DOW_select_next_item();
                break;
            case 40:
                inline_DOW_select_handle_down_arrow();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}


/*****************************************************************************
 * FUNCTION
 *  register_inline_DOW_select_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_inline_DOW_select_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(inline_DOW_select_previous_item, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_DOW_select_next_item, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_DOW_select_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_DOW_select_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_DOW_select_handle_up_arrow, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(inline_DOW_select_handle_down_arrow, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    register_keyboard_key_handler(inline_DOW_select_keyboard_key_handler);
}


/*****************************************************************************
 * FUNCTION
 *  redraw_inline_DOW_select
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_inline_DOW_select(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        show_DOW_select_background(&MMI_inline_DOW_select);
    }
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
     
    show_inline_DOW_select();
}

/* History implementation for inline DOW select */

typedef struct _inline_DOW_select_history
{
    U16 history_ID;
    S16 highlight_index;
    U32 flags;
} inline_DOW_select_history;


/*****************************************************************************
 * FUNCTION
 *  inline_DOW_select_get_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 *inline_DOW_select_get_history(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        U16 hID = (U16) (INLINE_DOW_SELECT_HISTORY_ID | 0x8000);
        inline_DOW_select_history *h = (inline_DOW_select_history*) history_buffer;

        h->history_ID = hID;
        h->highlight_index = (S16) MMI_inline_DOW_select.highlighted_cell;
        h->flags = MMI_inline_DOW_select.flags;
    }
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  inline_DOW_select_set_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  
 *****************************************************************************/
U8 inline_DOW_select_set_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        inline_DOW_select_history *h = (inline_DOW_select_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            MMI_inline_DOW_select.flags = h->flags;
            MMI_inline_DOW_select.highlighted_cell = (U8) h->highlight_index;
            return (1);
        }
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  initialize_inline_DOW_select
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  list_of_states      [?]         
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void initialize_inline_DOW_select(S32 x, S32 y, S32 width, S32 height, U8 *list_of_states, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    create_DOW_select(&MMI_inline_DOW_select, x, y, width, height);
    for (i = 0; i < 7; i++)
    {
        MMI_inline_DOW_select.states[i] = list_of_states[i];
    }
    current_wgui_inline_DOW_states = list_of_states;
    inline_DOW_select_set_history(INLINE_DOW_SELECT_HISTORY_ID, history_buffer);
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  inline_DOW_select_pen_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pen_event       [IN]        
 *  x               [IN]        
 *  y               [IN]        
 *  item_event      [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL inline_DOW_select_pen_handler(
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_inline_item_pen_enum *item_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret;
    DOW_pen_event_enum DOW_event;
    gui_pen_event_param_struct DOW_param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ret = DOW_translate_pen_event(&MMI_inline_DOW_select, pen_event, x, y, &DOW_event, &DOW_param);

    if (ret)
    {
        switch (DOW_event)
        {
            case DOW_PEN_CHANGE_HIGHLIGHT_AND_TOGGLE_I:
                DOW_select_highlight_item(&MMI_inline_DOW_select, DOW_param._u.i);
                /* FALLTHROUGH no break */
            case DOW_PEN_TOGGLE_CURRENT_ITEM:
                DOW_select_toggle_item(&MMI_inline_DOW_select);
                 
        #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
                if (IsInlineItemDefaultTextEnable())
                {
                    show_DOW_select_background(&MMI_inline_DOW_select);
                }
        #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
                 
                show_inline_DOW_select();
                handle_inline_DOW_select_LSK_display();
                set_wgui_inline_list_menu_changed();
                break;
            case DOW_PEN_NONE:
                /* Do nothing */
                break;
            default:
                MMI_ASSERT(0);
        }
    }

    *item_event = GUI_INLINE_ITEM_PEN_NONE;
    return ret;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  handle_inline_DOW_select
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_DOW_select(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_DOW_select *i = (wgui_inline_item_DOW_select*) item;
    S32 xx;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(flags);
    wgui_current_inline_item_type = INLINE_ITEM_TYPE_DOW_SELECT;
    if (MMI_fixed_icontext_menuitem.text_x == 2)
    {
        xx = -1;
    }
    else
    {
        xx = MMI_fixed_icontext_menuitem.text_x;
    }
    if (flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    clear_inline_item_keys = clear_inline_DOW_select_keys;
    current_wgui_inline_DOW_item = i;
    current_wgui_inline_DOW_text_p = text_p;
     
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    if (line_draw)
    {
        initialize_inline_DOW_select(x + xx, y + 1, width - xx - 1, height - 2, i->list_of_states, history_buffer);
    }
    else
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
         
         
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        initialize_inline_DOW_select(x, y, width, height, i->list_of_states, history_buffer);
    }
    else
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
         
        initialize_inline_DOW_select(x + xx, y, width - xx, height, i->list_of_states, history_buffer);
     
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        show_DOW_select_background(&MMI_inline_DOW_select);
    }
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
     
    show_inline_DOW_select();
    register_inline_DOW_select_keys();
    complete_inline_item_edit = inline_DOW_select_change_complete;
    redraw_current_inline_item = redraw_inline_DOW_select;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = inline_DOW_select_pen_handler;
#endif 
    handle_inline_DOW_select_LSK_display();
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemDOWSelect
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                [?]         
 *  item_index          [IN]        
 *  list_of_states      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemDOWSelect(InlineItem *item, S32 item_index, U8 *list_of_states)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_DOW_select *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_inline_DOW_items >= MAX_INLINE_DOW_SELECT_ITEMS)
    {
        return;
    }
    item->item = (void*)&wgui_inline_DOW_select_items[n_inline_DOW_items];
    item->flags = INLINE_ITEM_TYPE_DOW_SELECT;
    n_inline_DOW_items++;
    i = (wgui_inline_item_DOW_select*) item->item;
    i->flags = 0;
    i->list_of_states = list_of_states;
    item->handler = handle_inline_DOW_select;
    MMI_fixed_icontext_menuitems[item_index].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
}


/*****************************************************************************
 * FUNCTION
 *  ReConfigureInlineItemDOWSelect
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                [?]         
 *  item_index          [IN]        
 *  list_of_states      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ReConfigureInlineItemDOWSelect(InlineItem *item, S32 item_index, U8 *list_of_states)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_DOW_select *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((item->flags & 0xff) != INLINE_ITEM_TYPE_DOW_SELECT)
    {
        return;
    }
    i = (wgui_inline_item_DOW_select*) item->item;
    i->flags = 0;
    i->list_of_states = list_of_states;
}

/* Displaying DOW select as a menu item in the fixed list menu */


/*****************************************************************************
 * FUNCTION
 *  wgui_show_DOW_list_menuitem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  width       [IN]        
 *  height      [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_DOW_list_menuitem(S32 x1, S32 y1, S32 width, S32 height, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 xx;
    wgui_inline_item_DOW_select *i = (wgui_inline_item_DOW_select*) item->item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_fixed_icontext_menuitem.text_x == 2)
    {
        xx = -1;
    }
    else
    {
        xx = MMI_fixed_icontext_menuitem.text_x;
    }
    if (i->flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    gdi_layer_lock_frame_buffer();
     
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        initialize_inline_DOW_select(x1, y1, width, height, i->list_of_states, NULL);
    }
    else
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
         
        initialize_inline_DOW_select(x1 + xx, y1, width - xx, height, i->list_of_states, NULL);
    set_inline_DOW_select_list_menu_theme();
     
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        inline_item_default_text_show_background((x1 - 1), y1, (x1 - 1) + (width - 1), y1 + height - 1);
    }
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
     
    show_inline_DOW_select();
    redraw_left_softkey();
    redraw_right_softkey();
    gdi_layer_unlock_frame_buffer();
}

/*----------------------------------------------------------------------------
   Inline time period edit implementation
----------------------------------------------------------------------------*/

#define MAX_INLINE_TIME_PERIOD_ITEMS                     2
#define MAX_INLINE_TIME_PERIOD_DISPLAY_STRING_SIZE          32

S32 n_inline_time_period_items = 0;
wgui_inline_item_time_period wgui_inline_time_period_items[MAX_INLINE_TIME_PERIOD_ITEMS];
U8 wgui_inline_time_period_display_strings[MAX_INLINE_TIME_PERIOD_ITEMS][MAX_INLINE_TIME_PERIOD_DISPLAY_STRING_SIZE];
wgui_inline_item_time_period *current_wgui_inline_time_period_item;
U8 **current_wgui_inline_time_period_text_p;
U8 wgui_inline_time_period_edit_hours_buffer1[3 * ENCODING_LENGTH];
U8 wgui_inline_time_period_edit_minutes_buffer1[3 * ENCODING_LENGTH];
U8 wgui_inline_time_period_edit_hours_buffer2[3 * ENCODING_LENGTH];
U8 wgui_inline_time_period_edit_minutes_buffer2[3 * ENCODING_LENGTH];
U8 *wgui_inline_time_period_AM_PM_flag1;
U8 *wgui_inline_time_period_AM_PM_flag2;
time_period_input wgui_inline_time_period_input;


/*****************************************************************************
 * FUNCTION
 *  handle_inline_time_period_edit_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_time_period_edit_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_inline_fixed_list_keys();
    gdi_layer_lock_frame_buffer();
    gui_strcpy(
        (UI_string_type) current_wgui_inline_time_period_item->hours_buffer1,
        (UI_string_type) wgui_inline_time_period_edit_hours_buffer1);
    gui_strcpy(
        (UI_string_type) current_wgui_inline_time_period_item->minutes_buffer1,
        (UI_string_type) wgui_inline_time_period_edit_minutes_buffer1);
    gui_strcpy(
        (UI_string_type) current_wgui_inline_time_period_item->hours_buffer2,
        (UI_string_type) wgui_inline_time_period_edit_hours_buffer2);
    gui_strcpy(
        (UI_string_type) current_wgui_inline_time_period_item->minutes_buffer2,
        (UI_string_type) wgui_inline_time_period_edit_minutes_buffer2);
    current_wgui_inline_time_period_item->AM_PM_flag1 = wgui_inline_time_period_AM_PM_flag1;
    current_wgui_inline_time_period_item->AM_PM_flag2 = wgui_inline_time_period_AM_PM_flag2;
    current_wgui_inline_time_period_item->string_construct_callback(
                                            *current_wgui_inline_time_period_text_p,
                                            current_wgui_inline_time_period_item->hours_buffer1,
                                            current_wgui_inline_time_period_item-> minutes_buffer1,
                                            current_wgui_inline_time_period_item->AM_PM_flag1,
                                            current_wgui_inline_time_period_item->hours_buffer2,
                                            current_wgui_inline_time_period_item-> minutes_buffer2,
                                            current_wgui_inline_time_period_item->AM_PM_flag2);
    MMI_fixed_icontext_menuitems[wgui_inline_item_highlighted_index].item_text =
        (UI_string_type) * current_wgui_inline_time_period_text_p;
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_close_input_box();
#endif 
    wgui_inline_item_handled = 0;
    wgui_text_menuitem_restart_scrolling();
    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    complete_inline_item_edit = UI_dummy_function;
    set_current_time_period_input(NULL);
    clear_time_period_input_callback();
#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
     
    //But in in-line editor, redrawing is needed when completing in-line item
#ifdef __MMI_WGUI_CSK_ENABLE__
    redraw_softkey(MMI_CENTER_SOFTKEY);
#endif 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
 //   gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_time_period_edit_cancel
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_time_period_edit_cancel(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_inline_fixed_list_keys();
    gdi_layer_lock_frame_buffer();
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_close_input_box();
#endif 
    wgui_inline_item_handled = 0;
    wgui_text_menuitem_restart_scrolling();
    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    complete_inline_item_edit = UI_dummy_function;
    set_current_time_period_input(NULL);
    clear_time_period_input_callback();
#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
     
    //But in in-line editor, redrawing is needed when completing in-line item
#ifdef __MMI_WGUI_CSK_ENABLE__
    redraw_softkey(MMI_CENTER_SOFTKEY);
#endif 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
  //  gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  redraw_inline_time_period_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_inline_time_period_edit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    time_period_input_show_background(&wgui_inline_time_period_input);
    show_time_period_input(&wgui_inline_time_period_input);
}


/*****************************************************************************
 * FUNCTION
 *  inline_time_period_edit_handle_down_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_time_period_edit_handle_down_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    handle_inline_time_period_edit_complete();
    inline_fixed_list_goto_next_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_time_period_edit_handle_up_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_time_period_edit_handle_up_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    handle_inline_time_period_edit_complete();
    inline_fixed_list_goto_previous_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_time_period_input_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  vkey_code       [IN]        
 *  key_state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_time_period_input_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 37:
                time_period_input_previous_character();
                break;
            case 38:
                inline_time_period_edit_handle_up_arrow();
                break;
            case 39:
                time_period_input_next_character();
                break;
            case 40:
                inline_time_period_edit_handle_down_arrow();
                break;
            case 36:    /* home */
                break;
            case 35:    /* end */
                break;
            case 33:    /* page up */
                break;
            case 34:    /* page down */
                break;
            case 45:
                time_period_input_toggle_insert_mode();
                break;
            case 46:
                time_period_input_delete_current_character();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_time_period_edit_input
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_time_period_edit_input(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_wgui_inline_list_menu_changed();
}

/* History implementation for inline time_period edit */

typedef struct _inline_time_period_edit_history
{
    U16 history_ID;
    S16 current_focus;
    S16 current_position;
    S16 dummy;  /* Added for history issue on hardware */
    U32 flags;
} inline_time_period_edit_history;


/*****************************************************************************
 * FUNCTION
 *  inline_time_period_edit_get_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 *inline_time_period_edit_get_history(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        U16 hID = (U16) (INLINE_TIME_PERIOD_HISTORY_ID | 0x8000);
        inline_time_period_edit_history *h = (inline_time_period_edit_history*) history_buffer;

        h->history_ID = hID;
        h->current_focus = (S16) wgui_inline_time_period_input.current_focus;
        if (wgui_inline_time_period_input.focus_input_box != NULL)
        {
            h->current_position =
                (S16) (wgui_inline_time_period_input.focus_input_box->current_text_p -
                       wgui_inline_time_period_input.focus_input_box->text);
        }
        h->flags = wgui_inline_time_period_input.flags;
    }
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  inline_time_period_edit_set_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  
 *****************************************************************************/
U8 inline_time_period_edit_set_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        inline_time_period_edit_history *h = (inline_time_period_edit_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            wgui_inline_time_period_input.current_focus = h->current_focus;
            time_period_input_reset_focus(&wgui_inline_time_period_input);
            time_period_input_set_focus(&wgui_inline_time_period_input);
            if (wgui_inline_time_period_input.focus_input_box != NULL)
            {
                wgui_inline_time_period_input.focus_input_box->current_text_p =
                    wgui_inline_time_period_input.focus_input_box->text + h->current_position;
            }
            wgui_inline_time_period_input.flags = h->flags;
            return (1);
        }
    }
    return (0);
}

extern UI_filled_area time_period_input_background_filler;


/*****************************************************************************
 * FUNCTION
 *  handle_inline_time_period_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_time_period_edit(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_time_period *i = (wgui_inline_item_time_period*) item;
    S32 xx;
    U32 dtflags = 0;

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_handwriting_area_struct stroke_area;
    mmi_pen_handwriting_area_struct ext_stroke;
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(flags);
    wgui_current_inline_item_type = INLINE_ITEM_TYPE_TIME_PERIOD;
     
    if (!r2lMMIFlag)
    {
        if (MMI_fixed_icontext_menuitem.text_x == 2)
        {
            xx = -1;
        }
        else
        {
            xx = MMI_fixed_icontext_menuitem.text_x;
        }
    }
    else
    {
        xx = 6;
    }
    /* CSD end */
    if (flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    switch (flags & INLINE_ITEM_JUSTIFY_MASK)
    {
        case INLINE_ITEM_RIGHT_JUSTIFY:
            dtflags |= TIME_PERIOD_INPUT_RIGHT_JUSTIFY;
            break;
        case INLINE_ITEM_CENTER_JUSTIFY:
            dtflags |= TIME_PERIOD_INPUT_CENTER_JUSTIFY;
            break;
    }
    if (flags & INLINE_ITEM_DISABLE_HIGHLIGHT)
    {
        time_period_input_background_filler = MMI_inputbox_normal_filler;       /* *MMI_fixed_list_menu.normal_filler; */
        time_period_input_background_filler.flags &= 0x00000fff;
    }
    else
    {
        time_period_input_background_filler = MMI_inputbox_normal_filler;       /* *MMI_fixed_icontext_menuitem.focussed_filler; */
        time_period_input_background_filler.flags &= 0x00000fff;
    }
    complete_inline_item_edit = handle_inline_time_period_edit_complete;
    current_wgui_inline_time_period_item = i;
    current_wgui_inline_time_period_text_p = text_p;
    gui_strcpy((UI_string_type) wgui_inline_time_period_edit_hours_buffer1, (UI_string_type) i->hours_buffer1);
    gui_strcpy((UI_string_type) wgui_inline_time_period_edit_minutes_buffer1, (UI_string_type) i->minutes_buffer1);
    gui_strcpy((UI_string_type) wgui_inline_time_period_edit_hours_buffer2, (UI_string_type) i->hours_buffer2);
    gui_strcpy((UI_string_type) wgui_inline_time_period_edit_minutes_buffer2, (UI_string_type) i->minutes_buffer2);
    wgui_inline_time_period_AM_PM_flag1 = i->AM_PM_flag1;
    wgui_inline_time_period_AM_PM_flag2 = i->AM_PM_flag2;
     
    if (!r2lMMIFlag)
    {
        create_time_period_input(
            &wgui_inline_time_period_input,
            x + xx,
            y,
            width - 1 - xx,
            height,
            dtflags,
            wgui_inline_time_period_edit_hours_buffer1,
            wgui_inline_time_period_edit_minutes_buffer1,
            wgui_inline_time_period_edit_hours_buffer2,
            wgui_inline_time_period_edit_minutes_buffer2);
    }
    else
    {
        create_time_period_input(
            &wgui_inline_time_period_input,
            x + xx,
            y,
            width - 1 - MMI_fixed_icontext_menuitem.text_x,
            height,
            dtflags,
            wgui_inline_time_period_edit_hours_buffer1,
            wgui_inline_time_period_edit_minutes_buffer1,
            wgui_inline_time_period_edit_hours_buffer2,
            wgui_inline_time_period_edit_minutes_buffer2);
    }
    /* CSD end */

    inline_time_period_edit_set_history(INLINE_TIME_PERIOD_HISTORY_ID, history_buffer);
    time_period_input_set_seperators(&wgui_inline_time_period_input, (UI_character_type) ':', (UI_character_type) '-');
    time_period_input_show_background(&wgui_inline_time_period_input);
    show_time_period_input(&wgui_inline_time_period_input);
    set_current_time_period_input(&wgui_inline_time_period_input);
    time_period_input_register_keys();
    register_keyboard_key_handler(inline_time_period_input_key_handler);
    SetKeyHandler(inline_time_period_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_time_period_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_time_period_edit_handle_down_arrow, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    SetKeyHandler(inline_time_period_edit_handle_up_arrow, KEY_VOL_UP, KEY_EVENT_DOWN);
    register_time_period_input_callback(handle_inline_time_period_edit_input);
    redraw_current_inline_item = redraw_inline_time_period_edit;
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    ext_stroke.x1 = 0;
    ext_stroke.y1 = 0;
    ext_stroke.x2 = UI_device_width - 1;
    ext_stroke.y2 = UI_device_height - 1;
#if 0
#else /* 0 */ 
    stroke_area.x1 = MMI_content_x;
    stroke_area.x2 = MMI_content_x + MMI_content_width - MMI_fixed_list_menu.vbar.width;
    stroke_area.y1 = MMI_title_y;
    stroke_area.y2 = MMI_title_y + MMI_content_height + MMI_title_height;
#endif /* 0 */ 
    mmi_pen_editor_setup_input_box(&stroke_area, &ext_stroke, INPUT_TYPE_NUMERIC, MMI_FALSE, PEN_EDITOR_TIME_PERIOD);
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemTimePeriod
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                [?]         
 *  hours_buffer1       [?]         
 *  minutes_buffer1     [?]         
 *  AM_PM_flag1         [?]         
 *  hours_buffer2       [?]         
 *  minutes_buffer2     [?]         
 *  AM_PM_flag2         [?]         
 *  f                   [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemTimePeriod(
        InlineItem *item,
        U8 *hours_buffer1,
        U8 *minutes_buffer1,
        U8 *AM_PM_flag1,
        U8 *hours_buffer2,
        U8 *minutes_buffer2,
        U8 *AM_PM_flag2,
        void (*f) (U8 *string_buffer, U8 *hours_buffer1, U8 *minutes_buffer1, U8 *AM_PM_flag1, U8 *hours_buffer2, U8 *minutes_buffer2, U8 *AM_PM_flag2))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_time_period *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_inline_time_period_items >= MAX_INLINE_TIME_PERIOD_ITEMS)
    {
        return;
    }
    item->item = (void*)&wgui_inline_time_period_items[n_inline_time_period_items];
    item->flags = INLINE_ITEM_TYPE_TIME_PERIOD;
    item->text_p = (U8*) wgui_inline_time_period_display_strings[n_inline_time_period_items];
    n_inline_time_period_items++;
    i = (wgui_inline_item_time_period*) item->item;
    i->hours_buffer1 = hours_buffer1;
    i->minutes_buffer1 = minutes_buffer1;
    i->AM_PM_flag1 = AM_PM_flag1;
    i->hours_buffer2 = hours_buffer2;
    i->minutes_buffer2 = minutes_buffer2;
    i->AM_PM_flag2 = AM_PM_flag2;
    i->flags = INLINE_ITEM_LEFT_JUSTIFY;
    i->string_construct_callback = f;
    i->string_construct_callback(
        item->text_p,
        i->hours_buffer1,
        i->minutes_buffer1,
        i->AM_PM_flag1,
        i->hours_buffer2,
        i->minutes_buffer2,
        i->AM_PM_flag2);
    item->handler = handle_inline_time_period_edit;
}


/*****************************************************************************
 * FUNCTION
 *  ReConfigureInlineItemTimePeriod
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                [?]         
 *  hours_buffer1       [?]         
 *  minutes_buffer1     [?]         
 *  AM_PM_flag1         [?]         
 *  hours_buffer2       [?]         
 *  minutes_buffer2     [?]         
 *  AM_PM_flag2         [?]         
 *  f                   [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ReConfigureInlineItemTimePeriod(
        InlineItem *item,
        U8 *hours_buffer1,
        U8 *minutes_buffer1,
        U8 *AM_PM_flag1,
        U8 *hours_buffer2,
        U8 *minutes_buffer2,
        U8 *AM_PM_flag2,
        void (*f) (U8 *string_buffer, U8 *hours_buffer1, U8 *minutes_buffer1, U8 *AM_PM_flag1, U8 *hours_buffer2, U8 *minutes_buffer2, U8 *AM_PM_flag2))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_time_period *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((item->flags & 0xff) != INLINE_ITEM_TYPE_TIME_PERIOD)
    {
        return;
    }
    i = (wgui_inline_item_time_period*) item->item;
    i->hours_buffer1 = hours_buffer1;
    i->minutes_buffer1 = minutes_buffer1;
    i->AM_PM_flag1 = AM_PM_flag1;
    i->hours_buffer2 = hours_buffer2;
    i->minutes_buffer2 = minutes_buffer2;
    i->AM_PM_flag2 = AM_PM_flag2;
    i->flags = 0;
    i->string_construct_callback = f;
    i->string_construct_callback(
        item->text_p,
        i->hours_buffer1,
        i->minutes_buffer1,
        i->AM_PM_flag1,
        i->hours_buffer2,
        i->minutes_buffer2,
        i->AM_PM_flag2);
}

/* Displaying time period edit as a menu item in the fixed list menu */

U8 wgui_inline_time_period_edit_hours_display_buffer1[3 * ENCODING_LENGTH];
U8 wgui_inline_time_period_edit_minutes_display_buffer1[3 * ENCODING_LENGTH];
U8 *wgui_inline_time_period_AM_PM_display_flag1;
U8 wgui_inline_time_period_edit_hours_display_buffer2[3 * ENCODING_LENGTH];
U8 wgui_inline_time_period_edit_minutes_display_buffer2[3 * ENCODING_LENGTH];
U8 *wgui_inline_time_period_AM_PM_display_flag2;
time_period_input wgui_inline_time_period_input_display;


/*****************************************************************************
 * FUNCTION
 *  wgui_show_inline_time_period_list_menuitem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  width       [IN]        
 *  height      [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_inline_time_period_list_menuitem(S32 x1, S32 y1, S32 width, S32 height, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_time_period *i = (wgui_inline_item_time_period*) item->item;
    S32 xx;
    U32 dtflags = TIME_INPUT_TYPE_HH_MM;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    if (MMI_fixed_icontext_menuitem.text_x == 2)
    {
        xx = -1;
    }
    else
    {
        xx = MMI_fixed_icontext_menuitem.text_x;
    }
    if (item->flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    switch (item->flags & INLINE_ITEM_JUSTIFY_MASK)
    {
        case INLINE_ITEM_RIGHT_JUSTIFY:
            dtflags |= DATE_INPUT_RIGHT_JUSTIFY;
            break;
        case INLINE_ITEM_CENTER_JUSTIFY:
            dtflags |= DATE_INPUT_CENTER_JUSTIFY;
            break;
    }
    gui_strcpy((UI_string_type) wgui_inline_time_period_edit_hours_display_buffer1, (UI_string_type) i->hours_buffer1);
    gui_strcpy(
        (UI_string_type) wgui_inline_time_period_edit_minutes_display_buffer1,
        (UI_string_type) i->minutes_buffer1);
    gui_strcpy((UI_string_type) wgui_inline_time_period_edit_hours_display_buffer2, (UI_string_type) i->hours_buffer2);
    gui_strcpy(
        (UI_string_type) wgui_inline_time_period_edit_minutes_display_buffer2,
        (UI_string_type) i->minutes_buffer2);
    wgui_inline_time_period_AM_PM_display_flag1 = i->AM_PM_flag1;
    wgui_inline_time_period_AM_PM_display_flag2 = i->AM_PM_flag2;
    wgui_set_date_time_input_unfocussed_theme();
    create_time_period_input(
        &wgui_inline_time_period_input_display,
        x1 + xx,
        y1,
        width - 1 - xx,
        height,
        dtflags,
        wgui_inline_time_period_edit_hours_display_buffer1,
        wgui_inline_time_period_edit_minutes_display_buffer1,
        wgui_inline_time_period_edit_hours_display_buffer2,
        wgui_inline_time_period_edit_minutes_display_buffer2);
    time_period_input_set_seperators(
        &wgui_inline_time_period_input_display,
        (UI_character_type) ':',
        (UI_character_type) '-');
    time_period_input_show_background(&wgui_inline_time_period_input_display);
    time_period_input_reset_focus(&wgui_inline_time_period_input_display);
    show_time_period_input(&wgui_inline_time_period_input_display);
    wgui_set_date_time_input_focussed_theme();
    redraw_left_softkey();
    redraw_right_softkey();
    gdi_layer_unlock_frame_buffer();
}

/*----------------------------------------------------------------------------
   Inline IP4 edit implementation
----------------------------------------------------------------------------*/

#define MAX_INLINE_IP4_ITEMS                    2
#define MAX_INLINE_IP4_DISPLAY_STRING_SIZE            48

S32 n_inline_IP4_items = 0;
wgui_inline_item_IP4 wgui_inline_IP4_items[MAX_INLINE_IP4_ITEMS];
U8 wgui_inline_IP4_display_strings[MAX_INLINE_IP4_ITEMS][MAX_INLINE_IP4_DISPLAY_STRING_SIZE];
wgui_inline_item_IP4 *current_wgui_inline_IP4_item;
U8 **current_wgui_inline_IP4_text_p;
U8 wgui_inline_IP4_edit_buffer1[4 * ENCODING_LENGTH];
U8 wgui_inline_IP4_edit_buffer2[4 * ENCODING_LENGTH];
U8 wgui_inline_IP4_edit_buffer3[4 * ENCODING_LENGTH];
U8 wgui_inline_IP4_edit_buffer4[4 * ENCODING_LENGTH];
IP4_input wgui_inline_IP4_input;


/*****************************************************************************
 * FUNCTION
 *  handle_inline_IP4_edit_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_IP4_edit_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_inline_fixed_list_keys();
    gdi_layer_lock_frame_buffer();
    gui_strcpy((UI_string_type) current_wgui_inline_IP4_item->b1, (UI_string_type) wgui_inline_IP4_edit_buffer1);
    gui_strcpy((UI_string_type) current_wgui_inline_IP4_item->b2, (UI_string_type) wgui_inline_IP4_edit_buffer2);
    gui_strcpy((UI_string_type) current_wgui_inline_IP4_item->b3, (UI_string_type) wgui_inline_IP4_edit_buffer3);
    gui_strcpy((UI_string_type) current_wgui_inline_IP4_item->b4, (UI_string_type) wgui_inline_IP4_edit_buffer4);
    current_wgui_inline_IP4_item->string_construct_callback(
                                    *current_wgui_inline_IP4_text_p,
                                    current_wgui_inline_IP4_item->b1,
                                    current_wgui_inline_IP4_item->b2,
                                    current_wgui_inline_IP4_item->b3,
                                    current_wgui_inline_IP4_item->b4);
    MMI_fixed_icontext_menuitems[wgui_inline_item_highlighted_index].item_text =
        (UI_string_type) * current_wgui_inline_IP4_text_p;
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_close_input_box();
#endif 
    wgui_inline_item_handled = 0;
    wgui_text_menuitem_restart_scrolling();
    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    complete_inline_item_edit = UI_dummy_function;
    set_current_IP4_input(NULL);
    clear_IP4_input_callback();
#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
     
    //But in in-line editor, redrawing is needed when completing in-line item
#ifdef __MMI_WGUI_CSK_ENABLE__
    redraw_softkey(MMI_CENTER_SOFTKEY);
#endif 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
  //  gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_IP4_edit_cancel
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_IP4_edit_cancel(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_inline_fixed_list_keys();
    gdi_layer_lock_frame_buffer();
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_close_input_box();
#endif 
    wgui_inline_item_handled = 0;
    wgui_text_menuitem_restart_scrolling();
    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    complete_inline_item_edit = UI_dummy_function;
    set_current_IP4_input(NULL);
    clear_IP4_input_callback();
#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
     
    //But in in-line editor, redrawing is needed when completing in-line item
#ifdef __MMI_WGUI_CSK_ENABLE__
    redraw_softkey(MMI_CENTER_SOFTKEY);
#endif 
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  redraw_inline_IP4_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_inline_IP4_edit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    IP4_input_show_background(&wgui_inline_IP4_input);
    show_IP4_input(&wgui_inline_IP4_input);
}


/*****************************************************************************
 * FUNCTION
 *  inline_IP4_edit_handle_down_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_IP4_edit_handle_down_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    handle_inline_IP4_edit_complete();
    inline_fixed_list_goto_next_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_IP4_edit_handle_up_arrow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_IP4_edit_handle_up_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    handle_inline_IP4_edit_complete();
    inline_fixed_list_goto_previous_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_IP4_input_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  vkey_code       [IN]        
 *  key_state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_IP4_input_key_handler(S32 vkey_code, S32 key_state)
{
#if(MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (key_state)
    {
        switch (vkey_code)
        {
            case 37:
                IP4_input_previous_character();
                break;
            case 38:
                inline_IP4_edit_handle_up_arrow();
                break;
            case 39:
                IP4_input_next_character();
                break;
            case 40:
                inline_IP4_edit_handle_down_arrow();
                break;
            case 36:    /* home */
                break;
            case 35:    /* end */
                break;
            case 33:    /* page up */
                break;
            case 34:    /* page down */
                break;
            case 45:
                IP4_input_toggle_insert_mode();
                break;
            case 46:
                IP4_input_delete_current_character();
                break;
        }
    }
#else /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
    UI_UNUSED_PARAMETER(vkey_code);
    UI_UNUSED_PARAMETER(key_state);
#endif /* (MMI_BUILD_TYPE == BUILD_TYPE_X86WIN32) */ 
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_IP4_edit_input
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_IP4_edit_input(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_wgui_inline_list_menu_changed();
}

/* History implementation for inline IP4 edit   */

typedef struct _inline_IP4_edit_history
{
    U16 history_ID;
    S16 current_focus;
    S16 current_position;
    S16 dummy;  /* Added for history issue on hardware */
    U32 flags;
} inline_IP4_edit_history;


/*****************************************************************************
 * FUNCTION
 *  inline_IP4_edit_get_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 *inline_IP4_edit_get_history(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        U16 hID = (U16) (INLINE_IP4_HISTORY_ID | 0x8000);
        inline_IP4_edit_history *h = (inline_IP4_edit_history*) history_buffer;

        h->history_ID = hID;
        h->current_focus = (S16) wgui_inline_IP4_input.current_focus;
        if (wgui_inline_IP4_input.focus_input_box != NULL)
        {
            h->current_position =
                (S16) (wgui_inline_IP4_input.focus_input_box->current_text_p -
                       wgui_inline_IP4_input.focus_input_box->text);
        }
        h->flags = wgui_inline_IP4_input.flags;
    }
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  inline_IP4_edit_set_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  
 *****************************************************************************/
U8 inline_IP4_edit_set_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (history_buffer != NULL)
    {
        inline_IP4_edit_history *h = (inline_IP4_edit_history*) history_buffer;

        if (history_valid(h->history_ID, history_ID))
        {
            wgui_inline_IP4_input.current_focus = h->current_focus;
            IP4_input_reset_focus(&wgui_inline_IP4_input);
            IP4_input_set_focus(&wgui_inline_IP4_input);
            if (wgui_inline_IP4_input.focus_input_box != NULL)
            {
                wgui_inline_IP4_input.focus_input_box->current_text_p =
                    wgui_inline_IP4_input.focus_input_box->text + h->current_position;
            }
            wgui_inline_IP4_input.flags = h->flags;
            return (1);
        }
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_IP4_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_IP4_edit(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_IP4 *i = (wgui_inline_item_IP4*) item;
    S32 xx;
    U32 dtflags = 0;

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_handwriting_area_struct stroke_area;
    mmi_pen_handwriting_area_struct ext_stroke;
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(flags);
    wgui_current_inline_item_type = INLINE_ITEM_TYPE_IP4_EDIT;
     
    if (!r2lMMIFlag)
    {
        if (MMI_fixed_icontext_menuitem.text_x == 2)
        {
            xx = -1;
        }
        else
        {
            xx = MMI_fixed_icontext_menuitem.text_x;
        }
    }
    else
    {
        xx = 1;
    }
    /* CSD end */
    if (flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    switch (flags & INLINE_ITEM_JUSTIFY_MASK)
    {
        case INLINE_ITEM_RIGHT_JUSTIFY:
            dtflags |= IP4_INPUT_RIGHT_JUSTIFY;
            break;
        case INLINE_ITEM_CENTER_JUSTIFY:
            dtflags |= IP4_INPUT_CENTER_JUSTIFY;
            break;
    }
    complete_inline_item_edit = handle_inline_IP4_edit_complete;
    current_wgui_inline_IP4_item = i;
    current_wgui_inline_IP4_text_p = text_p;
    gui_strcpy((UI_string_type) wgui_inline_IP4_edit_buffer1, (UI_string_type) i->b1);
    gui_strcpy((UI_string_type) wgui_inline_IP4_edit_buffer2, (UI_string_type) i->b2);
    gui_strcpy((UI_string_type) wgui_inline_IP4_edit_buffer3, (UI_string_type) i->b3);
    gui_strcpy((UI_string_type) wgui_inline_IP4_edit_buffer4, (UI_string_type) i->b4);

    /* Ignore r2lMMIFlag */
    create_IP4_input(
        &wgui_inline_IP4_input,
        x + 1,
        y,
        width - 1,
        height,
        dtflags,
        wgui_inline_IP4_edit_buffer1,
        wgui_inline_IP4_edit_buffer2,
        wgui_inline_IP4_edit_buffer3,
        wgui_inline_IP4_edit_buffer4);

    inline_IP4_edit_set_history(INLINE_IP4_HISTORY_ID, history_buffer);
    IP4_input_set_seperator(&wgui_inline_IP4_input, (UI_character_type) '.');
    IP4_input_show_background(&wgui_inline_IP4_input);
    show_IP4_input(&wgui_inline_IP4_input);
    set_current_IP4_input(&wgui_inline_IP4_input);
    IP4_input_register_keys();
    register_keyboard_key_handler(inline_IP4_input_key_handler);
    SetKeyHandler(inline_IP4_edit_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_IP4_edit_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_IP4_edit_handle_down_arrow, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    SetKeyHandler(inline_IP4_edit_handle_up_arrow, KEY_VOL_UP, KEY_EVENT_DOWN);
    register_IP4_input_callback(handle_inline_IP4_edit_input);
    redraw_current_inline_item = redraw_inline_IP4_edit;
    UI_UNUSED_PARAMETER(xx);
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    ext_stroke.x1 = 0;
    ext_stroke.y1 = 0;
    ext_stroke.x2 = UI_device_width - 1;
    ext_stroke.y2 = UI_device_height - 1;
#if 0
#else /* 0 */ 
    stroke_area.x1 = MMI_content_x;
    stroke_area.x2 = MMI_content_x + MMI_content_width - MMI_fixed_list_menu.vbar.width;
    stroke_area.y1 = MMI_title_y;
    stroke_area.y2 = MMI_title_y + MMI_content_height + MMI_title_height;
#endif /* 0 */ 
    mmi_pen_editor_setup_input_box(&stroke_area, &ext_stroke, INPUT_TYPE_NUMERIC, MMI_FALSE, PEN_EDITOR_IP4);
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemIP4
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  b1          [?]         
 *  b2          [?]         
 *  b3          [?]         
 *  b4          [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemIP4(
        InlineItem *item,
        U8 *b1,
        U8 *b2,
        U8 *b3,
        U8 *b4,
        void (*f) (U8 *string_buffer, U8 *b1, U8 *b2, U8 *b3, U8 *b4))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_IP4 *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_inline_IP4_items >= MAX_INLINE_IP4_ITEMS)
    {
        return;
    }
    item->item = (void*)&wgui_inline_IP4_items[n_inline_IP4_items];
    item->flags = INLINE_ITEM_TYPE_IP4_EDIT;
    item->text_p = (U8*) wgui_inline_IP4_display_strings[n_inline_IP4_items];
    n_inline_IP4_items++;
    i = (wgui_inline_item_IP4*) item->item;
    i->b1 = b1;
    i->b2 = b2;
    i->b3 = b3;
    i->b4 = b4;
    i->flags = INLINE_ITEM_LEFT_JUSTIFY;
    i->string_construct_callback = f;
    i->string_construct_callback(item->text_p, i->b1, i->b2, i->b3, i->b4);
    item->handler = handle_inline_IP4_edit;
}


/*****************************************************************************
 * FUNCTION
 *  ReConfigureInlineItemIP4
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  b1          [?]         
 *  b2          [?]         
 *  b3          [?]         
 *  b4          [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ReConfigureInlineItemIP4(
        InlineItem *item,
        U8 *b1,
        U8 *b2,
        U8 *b3,
        U8 *b4,
        void (*f) (U8 *string_buffer, U8 *b1, U8 *b2, U8 *b3, U8 *b4))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_IP4 *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((item->flags & 0xff) != INLINE_ITEM_TYPE_IP4_EDIT)
    {
        return;
    }
    i = (wgui_inline_item_IP4*) item->item;
    i->b1 = b1;
    i->b2 = b2;
    i->b3 = b3;
    i->b4 = b4;
    i->flags = 0;
    i->string_construct_callback = f;
    i->string_construct_callback(item->text_p, i->b1, i->b2, i->b3, i->b4);
}

/* Displaying IP address edit as a menu item in the fixed list menu  */

U8 wgui_inline_IP4_edit_display_buffer1[4 * ENCODING_LENGTH];
U8 wgui_inline_IP4_edit_display_buffer2[4 * ENCODING_LENGTH];
U8 wgui_inline_IP4_edit_display_buffer3[4 * ENCODING_LENGTH];
U8 wgui_inline_IP4_edit_display_buffer4[4 * ENCODING_LENGTH];
IP4_input wgui_inline_IP4_input_display;


/*****************************************************************************
 * FUNCTION
 *  wgui_show_inline_IP4_list_menuitem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  width       [IN]        
 *  height      [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_inline_IP4_list_menuitem(S32 x1, S32 y1, S32 width, S32 height, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_IP4 *i = (wgui_inline_item_IP4*) item->item;
    S32 xx;
    U32 dtflags = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    if (MMI_fixed_icontext_menuitem.text_x == 2)
    {
        xx = -1;
    }
    else
    {
        xx = MMI_fixed_icontext_menuitem.text_x;
    }
    if (item->flags & INLINE_ITEM_LEFT_ALIGN)
    {
        xx = -1;
    }
    switch (item->flags & INLINE_ITEM_JUSTIFY_MASK)
    {
        case INLINE_ITEM_RIGHT_JUSTIFY:
            dtflags |= DATE_INPUT_RIGHT_JUSTIFY;
            break;
        case INLINE_ITEM_CENTER_JUSTIFY:
            dtflags |= DATE_INPUT_CENTER_JUSTIFY;
            break;
    }
    gui_strcpy((UI_string_type) wgui_inline_IP4_edit_display_buffer1, (UI_string_type) i->b1);
    gui_strcpy((UI_string_type) wgui_inline_IP4_edit_display_buffer2, (UI_string_type) i->b2);
    gui_strcpy((UI_string_type) wgui_inline_IP4_edit_display_buffer3, (UI_string_type) i->b3);
    gui_strcpy((UI_string_type) wgui_inline_IP4_edit_display_buffer4, (UI_string_type) i->b4);
    wgui_set_date_time_input_unfocussed_theme();
    create_IP4_input(
        &wgui_inline_IP4_input_display,
        x1 - 1,
        y1,
        width + 1,
        height,
        dtflags,
        wgui_inline_IP4_edit_display_buffer1,
        wgui_inline_IP4_edit_display_buffer2,
        wgui_inline_IP4_edit_display_buffer3,
        wgui_inline_IP4_edit_display_buffer4);
    IP4_input_set_seperator(&wgui_inline_IP4_input_display, (UI_character_type) '.');
     
    /* show the different background in case of default text effect is enabled
       for the inline item type IP4 Edit. */
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        inline_item_default_text_show_background(
            wgui_inline_IP4_input_display.x,
            wgui_inline_IP4_input_display.y,
            wgui_inline_IP4_input_display.x + wgui_inline_IP4_input_display.width - 1,
            wgui_inline_IP4_input_display.y + wgui_inline_IP4_input_display.height - 1);
    }
    else
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
         
        IP4_input_show_background(&wgui_inline_IP4_input_display);
    IP4_input_reset_focus(&wgui_inline_IP4_input_display);
    show_IP4_input(&wgui_inline_IP4_input_display);
    wgui_set_date_time_input_focussed_theme();
    redraw_left_softkey();
    redraw_right_softkey();
    gdi_layer_unlock_frame_buffer();
    UI_UNUSED_PARAMETER(xx);
}

/*****************************************************************************
*  
* Inline Color Selection
*
*****************************************************************************/
#define MAX_INLINE_COLOR_SELECT_ITEMS        3
#define INLINE_COLOR_SELECT_HEIGHT_SPACING   3
#define INLINE_COLOR_SELECT_WIDTH_SPACING    15
#define INLINE_COLOR_SELECT_ICON_SPACING     3

U16 n_inline_color_select_items = 0;
U16 current_wgui_inline_color_select_highlighted_index;
U16 current_wgui_inline_color_select_start_index;
U16 current_wgui_inline_color_select_region_x1;
U16 current_wgui_inline_color_select_region_y1;
U16 current_wgui_inline_color_select_region_x2;
U16 current_wgui_inline_color_select_region_y2;

U16 current_wgui_inline_color_select_icon_y;
U16 current_wgui_inline_color_select_icon1_x1;
U16 current_wgui_inline_color_select_icon1_x2;
U16 current_wgui_inline_color_select_icon2_x1;
U16 current_wgui_inline_color_select_icon2_x2;
U8 current_wgui_inline_color_select_icon1_pressed;
U8 current_wgui_inline_color_select_icon2_pressed;
U8 current_wgui_inline_color_select_pen_on_icon1;
U8 current_wgui_inline_color_select_pen_on_icon2;

U16 current_wgui_inline_color_select_box_count;
U16 current_wgui_inline_color_select_box_size;
U16 current_wgui_inline_color_select_box_spacing;

wgui_inline_item_color_select wgui_inline_color_select_items[MAX_INLINE_COLOR_SELECT_ITEMS];
wgui_inline_item_color_select *current_wgui_inline_color_select_item = NULL;


/*****************************************************************************
 * FUNCTION
 *  clear_inline_color_select_menu_keys
 * DESCRIPTION
 *  clear color inline select key handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void clear_inline_color_select_menu_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

    clear_keyboard_key_handler();
    clear_inline_item_keys = UI_dummy_function;
    UI_text_menuitem_scroll_time = UI_TEXT_MENUITEM_SCROLL_TIME;
}


/*****************************************************************************
 * FUNCTION
 *  inline_color_select_menu_change_complete
 * DESCRIPTION
 *  move out from inline color select
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void inline_color_select_menu_change_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    clear_inline_color_select_menu_keys();
    wgui_inline_item_handled = 0;
    redraw_current_inline_item = UI_dummy_function;
    complete_inline_item_edit = UI_dummy_function;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = UI_dummy_inline_item_pen_function;
#endif 

    current_wgui_inline_color_select_highlighted_index = 0;
}


/*****************************************************************************
 * FUNCTION
 *  inline_color_select_menu_handle_up_arrow
 * DESCRIPTION
 *  inline color select up arrow handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void inline_color_select_menu_handle_up_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_color_select_menu_change_complete();
    inline_fixed_list_goto_previous_item();
}


/*****************************************************************************
 * FUNCTION
 *  inline_color_select_menu_handle_down_arrow
 * DESCRIPTION
 *  inline color select down arrow handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void inline_color_select_menu_handle_down_arrow(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_color_select_menu_change_complete();
    inline_fixed_list_goto_next_item();
}


/*****************************************************************************
 * FUNCTION
 *  show_highlighted_inline_color_select_menuitem
 * DESCRIPTION
 *  draw inline color select menu when it is highlighted
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void show_highlighted_inline_color_select_menuitem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 box_count;  /* how many color will display */
    S32 box_size;
    S32 spacing;
    S32 i;
    S32 box_x1;
    S32 box_x2;
    S32 box_y1;
    S32 box_y2;
    S32 start_x;
    U16 start_index;
    U16 highlighted_index;
    U16 color_count;
    U16 color_index;
    color_t *color_list;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /*  name is too long, hard to read, set to local variable first */
    box_size = current_wgui_inline_color_select_box_size;
    box_count = current_wgui_inline_color_select_box_count;
    spacing = current_wgui_inline_color_select_box_spacing;

    box_y1 = current_wgui_inline_color_select_region_y1;
    box_y2 = current_wgui_inline_color_select_region_y2;
    start_x = current_wgui_inline_color_select_region_x1;

    start_index = current_wgui_inline_color_select_item->start_index;
    highlighted_index = current_wgui_inline_color_select_item->highlighted_index;
    color_list = current_wgui_inline_color_select_item->list_of_color;
    color_count = current_wgui_inline_color_select_item->n_color;

    /* need adjst start index */
    if (start_index == 255)
    {
        /* total color is less then box, always start at 0 */
        if (box_count > color_count)
        {
            start_index = 0;
        }
        else
        {
            if (color_count - highlighted_index >= box_count)
            {
                /* remain color is more than box count */
                start_index = highlighted_index;
            }
            else
            {
                /* remain color not enough */
                start_index = color_count - box_count;
            }

            current_wgui_inline_color_select_item->start_index = start_index;
        }
    }

     
    /* fill the background of the focussed Color Select menu item with modified
       parameters. */
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
        /* fill region with selected background color */
    {
        gui_fill_rectangle(
            current_wgui_inline_color_select_region_x1 - spacing,
            current_wgui_inline_color_select_region_y1 - spacing,
            current_wgui_inline_color_select_region_x2 + spacing,
            current_wgui_inline_color_select_region_y2 + spacing,
            current_MMI_theme->list_selected_item_filler->c);
    }
    else
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
         
        /* fill region with selected background color */
        gui_fill_rectangle(
            current_wgui_inline_color_select_icon1_x1 - 2,
            current_wgui_inline_color_select_region_y1 - 2,
            current_wgui_inline_color_select_icon2_x2 + 2,
            current_wgui_inline_color_select_region_y2 + 2,
            current_MMI_theme->list_selected_item_filler->c);

    for (i = 0; i < box_count; i++)
    {
        box_x1 = start_x + (box_size + spacing) * i;
        box_x2 = box_x1 + box_size;

        color_index = (U16) (start_index + i);

        if (color_index > color_count)
        {
            /* no more, get from firsr */
            color_index = color_index - color_count;
        }

        gui_fill_rectangle(box_x1, box_y1, box_x2, box_y2, color_list[color_index]);

        /* if is pure blue, draw cross */
        if (color_list[color_index].r == 0 && color_list[color_index].g == 0 && color_list[color_index].b == 255)
        {
            gui_fill_rectangle(box_x1, box_y1, box_x2, box_y2, gui_color(255, 255, 255));
            gui_line(box_x1, box_y1, box_x2, box_y2, gui_color(0, 0, 0));
            gui_line(box_x2, box_y1, box_x1, box_y2, gui_color(0, 0, 0));
        }
        else
        {
            gui_fill_rectangle(box_x1, box_y1, box_x2, box_y2, color_list[color_index]);
        }

        gui_draw_rectangle(box_x1 + 1, box_y1 + 1, box_x2 - 1, box_y2 - 1, gui_color(255, 255, 255));

        if (color_index == highlighted_index)
        {
            gui_draw_rectangle(box_x1, box_y1, box_x2, box_y2, gui_color(255, 0, 0));
            gui_draw_rectangle(box_x1 - 1, box_y1 - 1, box_x2 + 1, box_y2 + 1, gui_color(255, 0, 0));
        }
        else
        {
            gui_draw_rectangle(box_x1, box_y1, box_x2, box_y2, gui_color(0, 0, 0));
        }
    }

    gui_show_image(
        current_wgui_inline_color_select_icon1_x1,
        current_wgui_inline_color_select_icon_y + current_wgui_inline_color_select_icon1_pressed,
        (PU8) GetImage(IMG_H_SELECT_LEFT));
    gui_show_image(
        current_wgui_inline_color_select_icon2_x1,
        current_wgui_inline_color_select_icon_y + current_wgui_inline_color_select_icon2_pressed,
        (PU8) GetImage(IMG_H_SELECT_RIGHT));

    gdi_layer_blt_previous(
        current_wgui_inline_color_select_icon1_x1 - 2,
        current_wgui_inline_color_select_region_y1 - 2,
        current_wgui_inline_color_select_icon2_x2 + 2,
        current_wgui_inline_color_select_region_y2 + 2);
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_highlighted_inline_color_select_menuitemitem
 * DESCRIPTION
 *  show color select menu which is not selected
 * PARAMETERS
 *  x           [IN]        Menuitem start position x
 *  y           [IN]        Menuitem start position y
 *  width       [IN]        Menuitem width
 *  height      [IN]        Menuitem height
 *  item        [IN]        Selected item ptr
 * RETURNS
 *  void
 *****************************************************************************/
static void wgui_show_highlighted_inline_color_select_menuitemitem(
                S32 x,
                S32 y,
                S32 width,
                S32 height,
                wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 spacing;
    S32 region_spacing;
    S32 box_x1;
    S32 box_x2;
    S32 region_x1;
    S32 region_y1;
    S32 region_x2;
    S32 region_y2;
    S32 box_size;
    S32 box_count;
    S32 i;
    U16 color_index;
    U16 color_count;
    U16 start_index;
    S32 used_width;
    S32 region_width;
    U16 highlighted_index;
    color_t *color_list;

     
    S32 icon_width, icon_height;

     

    wgui_inline_item_color_select *item_ptr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item_ptr = (wgui_inline_item_color_select*) item->item;

    spacing = INLINE_COLOR_SELECT_HEIGHT_SPACING;
     
    /* removed the hard-coded value of region_spacing. */
    /* region_spacing = INLINE_COLOR_SELECT_WIDTH_SPACING; */
    gui_measure_image((PU8) GetImage(IMG_H_SELECT_LEFT), &icon_width, &icon_height);
    region_spacing = icon_width + (2 * INLINE_COLOR_SELECT_ICON_SPACING);
     

     
    if (r2lMMIFlag)
    {
        region_x1 = x + region_spacing - 4;
        region_x2 = x + width - MMI_fixed_icontext_menuitem.text_x - region_spacing;
    }
    else
    {
        region_x1 = x + MMI_fixed_icontext_menuitem.text_x + region_spacing - 4;        /* -4 for arrow width */
        region_x2 = x + width - region_spacing;
    }

#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        region_x1 = x + region_spacing;
        region_x2 = x + width - region_spacing;
    }
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
    /* region_x1 and region_x2 are moved above. */
    // region_x1   = x+MMI_fixed_icontext_menuitem.text_x+region_spacing-4; /* -4 for arrow width */
    region_y1 = y + spacing;
    /* region_x2   = x+width-region_spacing; */
    region_y2 = y + height - spacing;
     

    highlighted_index = item_ptr->highlighted_index;
    start_index = item_ptr->start_index;
    color_list = item_ptr->list_of_color;
    color_count = item_ptr->n_color;
    box_size = region_y2 - region_y1;
    box_count = (region_x2 - region_x1 + spacing) / (box_size + spacing);

    /* need adjst start index */
    if (start_index == 255)
    {
        /* total color is less then box, always start at 0 */
        if (box_count > color_count)
        {
            start_index = 0;
        }
        else
        {
            if (color_count - highlighted_index >= box_count)
            {
                /* remain color is more than box count */
                start_index = highlighted_index;
            }
            else
            {
                /* remain color not enough */
                start_index = (U16) (color_count - box_count);
            }

            item_ptr->start_index = start_index;
        }
    }

    /* dispatch remain spacing to both side */
    used_width = box_size * box_count + (box_count - 1) * spacing;
    region_width = region_x2 - region_x1 + 1;

    region_x1 += (region_width - used_width) >> 1;

    /* if total color is less then box count, adjust position */
    if (box_count > color_count)
    {
        box_count = color_count;

        /* adjust to center */
        used_width = box_size * box_count + (box_count - 1) * spacing;
        region_width = region_x2 - region_x1 + 1;

        region_x1 += (region_width - used_width) >> 1;
    }

     
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        inline_item_default_text_show_background((x - 1), y, (x - 1) + (width - 1), y + height);
    }
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
     

    for (i = 0; i < box_count; i++)
    {
        box_x1 = region_x1 + (box_size + spacing) * i;
        box_x2 = box_x1 + box_size;

        color_index = (U16) (start_index + i);

        if (color_index > color_count)
        {
            /* no more, get from firsr */
            color_index = color_index - color_count;
        }

            /* if is pure blue, draw cross */
            if (color_list[color_index].r == 0 && color_list[color_index].g == 0 && color_list[color_index].b == 255)
            {
                gui_fill_rectangle(box_x1, region_y1, box_x2, region_y2, gui_color(255, 255, 255));
                gui_line(box_x1, region_y1, box_x2, region_y2, gui_color(0, 0, 0));
                gui_line(box_x2, region_y1, box_x1, region_y2, gui_color(0, 0, 0));
            }
            else
            {
                gui_fill_rectangle(box_x1, region_y1, box_x2, region_y2, color_list[color_index]);
            }

        if (color_index == highlighted_index)
        {
            gui_draw_rectangle(box_x1, region_y1, box_x2, region_y2, gui_color(255, 0, 0));
            gui_draw_rectangle(box_x1 - 1, region_y1 - 1, box_x2 + 1, region_y2 + 1, gui_color(255, 0, 0));
        }
        else
        {
            gui_draw_rectangle(box_x1, region_y1, box_x2, region_y2, gui_color(0, 0, 0));
        }

        gui_draw_rectangle(box_x1 + 1, region_y1 + 1, box_x2 - 1, region_y2 - 1, gui_color(255, 255, 255));

    }

}


/*****************************************************************************
 * FUNCTION
 *  inline_color_select_menu_previous_item
 * DESCRIPTION
 *  selection change to previous color
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void inline_color_select_menu_previous_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (current_wgui_inline_color_select_item->highlighted_index == 0)
    {
        current_wgui_inline_color_select_item->highlighted_index = current_wgui_inline_color_select_item->n_color - 1;

        current_wgui_inline_color_select_item->start_index =
            current_wgui_inline_color_select_item->n_color - current_wgui_inline_color_select_box_count;
    }
    else
    {
        current_wgui_inline_color_select_item->highlighted_index--;
        if (current_wgui_inline_color_select_item->highlighted_index <
            current_wgui_inline_color_select_item->start_index)
        {
            current_wgui_inline_color_select_item->start_index--;
        }
    }

    show_highlighted_inline_color_select_menuitem();
    if (current_wgui_inline_color_select_item->color_highlighted != NULL)
    {
        current_wgui_inline_color_select_item->color_highlighted(current_wgui_inline_color_select_item->
                                                                 highlighted_index);
    }

}


/*****************************************************************************
 * FUNCTION
 *  inline_color_select_menu_next_item
 * DESCRIPTION
 *  selection change to next color
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void inline_color_select_menu_next_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (current_wgui_inline_color_select_item->highlighted_index >= current_wgui_inline_color_select_item->n_color - 1)
    {
        current_wgui_inline_color_select_item->highlighted_index = 0;
        current_wgui_inline_color_select_item->start_index = 0;

    }
    else
    {
        current_wgui_inline_color_select_item->highlighted_index++;

        if (current_wgui_inline_color_select_item->highlighted_index >=
            current_wgui_inline_color_select_item->start_index + current_wgui_inline_color_select_box_count)
        {
            current_wgui_inline_color_select_item->start_index++;
        }
    }

    show_highlighted_inline_color_select_menuitem();
    if (current_wgui_inline_color_select_item->color_highlighted != NULL)
    {
        current_wgui_inline_color_select_item->color_highlighted(current_wgui_inline_color_select_item->
                                                                 highlighted_index);
    }

}


/*****************************************************************************
 * FUNCTION
 *  register_inline_color_select_menu_keys
 * DESCRIPTION
 *  register key handler when highlighted on inline color selection
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void register_inline_color_select_menu_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetKeyHandler(inline_color_select_menu_previous_item, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_color_select_menu_next_item, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_color_select_menu_handle_up_arrow, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_color_select_menu_handle_down_arrow, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(inline_color_select_menu_handle_up_arrow, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(inline_color_select_menu_handle_down_arrow, KEY_VOL_DOWN, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  inline_color_select_pen_handler
 * DESCRIPTION
 *  Pen handler of color picker
 * PARAMETERS
 *  pen_event           [IN]        
 *  x                   [IN]        
 *  y                   [IN]        
 *  inline_event        [?]         
 *  item_event(?)       [OUT]       
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_TOUCH_SCREEN__
static BOOL inline_color_select_pen_handler(
                mmi_pen_event_type_enum pen_event,
                S16 x,
                S16 y,
                gui_inline_item_pen_enum *inline_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret;
    S32 i;
    S32 box_xoffset, box_width;
    S32 x1, y1, x2, y2, region_width, region_height;
    BOOL in_left_arrow, in_right_arrow;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *inline_event = GUI_INLINE_PEN_NONE;
    ret = MMI_TRUE;

    x1 = current_wgui_inline_color_select_region_x1;
    y1 = current_wgui_inline_color_select_region_y1;
    x2 = current_wgui_inline_color_select_region_x2;
    y2 = current_wgui_inline_color_select_region_y2;
    region_width = x2 - x1 + 1;
    region_height = y2 - y1 + 1;

    in_left_arrow = MMI_FALSE;
    in_right_arrow = MMI_FALSE;
     
    // if (x < x1 && y >= current_wgui_inline_color_select_region_y1 && y <= current_wgui_inline_color_select_region_y2)
    if (x >= current_wgui_inline_color_select_icon1_x1
        && x <= current_wgui_inline_color_select_icon1_x2
        && y >= current_wgui_inline_color_select_region_y1 && y <= current_wgui_inline_color_select_region_y2)
    {
        in_left_arrow = MMI_TRUE;
    }
    /* else if (x > x2 && y >= current_wgui_inline_color_select_region_y1 && y <= current_wgui_inline_color_select_region_y2) */
    else if (x >= current_wgui_inline_color_select_icon2_x1
             && x <= current_wgui_inline_color_select_icon2_x2
             && y >= current_wgui_inline_color_select_region_y1 && y <= current_wgui_inline_color_select_region_y2)
         
    {
        in_right_arrow = MMI_TRUE;
    }

    switch (pen_event)
    {
        case MMI_PEN_EVENT_DOWN:
            current_wgui_inline_color_select_icon1_pressed = 0;
            current_wgui_inline_color_select_icon2_pressed = 0;
            current_wgui_inline_color_select_pen_on_icon1 = 0;
            current_wgui_inline_color_select_pen_on_icon2 = 0;

            if (in_left_arrow)
            {
                current_wgui_inline_color_select_pen_on_icon1 = 1;
                current_wgui_inline_color_select_icon1_pressed = 1;
                show_highlighted_inline_color_select_menuitem();
            }
            else if (in_right_arrow)
            {
                current_wgui_inline_color_select_pen_on_icon2 = 1;
                current_wgui_inline_color_select_icon2_pressed = 1;
                show_highlighted_inline_color_select_menuitem();
            }
            else if (PEN_CHECK_BOUND(x, y, x1, y1, region_width, region_height))
            {
                box_width = current_wgui_inline_color_select_box_size + current_wgui_inline_color_select_box_spacing;
                box_xoffset = x1 + box_width - 1;
                for (i = 0; i < current_wgui_inline_color_select_box_count; i++)
                {
                    if (box_xoffset > x)
                    {
                        current_wgui_inline_color_select_item->highlighted_index =
                            i + current_wgui_inline_color_select_item->start_index;
                        show_highlighted_inline_color_select_menuitem();
                        if (current_wgui_inline_color_select_item->color_highlighted != NULL)
                        {
                            current_wgui_inline_color_select_item->
                                color_highlighted(current_wgui_inline_color_select_item->highlighted_index);
                        }

                        break;
                    }

                    box_xoffset += box_width;
                }
            }
            break;

        case MMI_PEN_EVENT_UP:
            if (current_wgui_inline_color_select_pen_on_icon1 && in_left_arrow)
            {
                inline_color_select_menu_previous_item();
            }
            else if (current_wgui_inline_color_select_pen_on_icon2 && in_right_arrow)
            {
                inline_color_select_menu_next_item();
            }

            if (current_wgui_inline_color_select_icon1_pressed || current_wgui_inline_color_select_icon2_pressed)
            {
                current_wgui_inline_color_select_icon1_pressed = 0;
                current_wgui_inline_color_select_icon2_pressed = 0;
                show_highlighted_inline_color_select_menuitem();
            }
            break;

        case MMI_PEN_EVENT_MOVE:
            if (current_wgui_inline_color_select_pen_on_icon1)
            {
                if (in_left_arrow)
                {
                    if (!current_wgui_inline_color_select_icon1_pressed)
                    {
                        current_wgui_inline_color_select_icon1_pressed = 1;
                        show_highlighted_inline_color_select_menuitem();
                    }
                }
                else
                {
                    if (current_wgui_inline_color_select_icon1_pressed)
                    {
                        current_wgui_inline_color_select_icon1_pressed = 0;
                        show_highlighted_inline_color_select_menuitem();
                    }
                }
            }
            else if (current_wgui_inline_color_select_pen_on_icon2)
            {
                if (in_right_arrow)
                {
                    if (!current_wgui_inline_color_select_icon2_pressed)
                    {
                        current_wgui_inline_color_select_icon2_pressed = 1;
                        show_highlighted_inline_color_select_menuitem();
                    }
                }
                else
                {
                    if (current_wgui_inline_color_select_icon2_pressed)
                    {
                        current_wgui_inline_color_select_icon2_pressed = 0;
                        show_highlighted_inline_color_select_menuitem();
                    }
                }
            }
            break;

        case MMI_PEN_EVENT_LONG_TAP:
            break;

        case MMI_PEN_EVENT_REPEAT:
            break;

        case MMI_PEN_EVENT_ABORT:
            if (current_wgui_inline_color_select_icon1_pressed || current_wgui_inline_color_select_icon2_pressed)
            {
                current_wgui_inline_color_select_icon1_pressed = 0;
                current_wgui_inline_color_select_icon2_pressed = 0;
                show_highlighted_inline_color_select_menuitem();
            }
            break;

        default:
            MMI_ASSERT(0);
    }

    return ret;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  handle_inline_color_select
 * DESCRIPTION
 *  inline color select hihight hanlder, will init required global variables for drawing.
 * PARAMETERS
 *  x                   [IN]        Menuitem start position x
 *  y                   [IN]        Menuitem start position y
 *  width               [IN]        Menuitem width
 *  height              [IN]        Menuitem height
 *  key_code            [IN]        Not used
 *  key_event           [IN]        Not used
 *  text_p              [IN]        Not used
 *  item                [IN]        Selected item pointer
 *  flags               [IN]        Not used
 *  history_buffer      [IN]        Not used
 * RETURNS
 *  void
 *****************************************************************************/
static void handle_inline_color_select(
                S32 x,
                S32 y,
                S32 width,
                S32 height,
                S32 key_code,
                S32 key_event,
                U8 **text_p,
                void *item,
                U32 flags,
                U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 spacing;
    S32 region_spacing;
    S32 icon_spacing, icon_width, icon_height;
    S32 used_width;
    S32 region_width;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    spacing = INLINE_COLOR_SELECT_HEIGHT_SPACING;
     
    // region_spacing = INLINE_COLOR_SELECT_WIDTH_SPACING;
     
    icon_spacing = INLINE_COLOR_SELECT_ICON_SPACING;

     
    /* removed the hard-coded value of region_spacing. Now it is calculated according to
       the icon width of arrow image. */
    gui_measure_image((PU8) GetImage(IMG_H_SELECT_LEFT), &icon_width, &icon_height);
    region_spacing = icon_width + (2 * icon_spacing);
     

    /* wgui_show_inline_select_arrow(x, y, width-3, height, &wgui_inline_items[current_fixed_list_menuitem_display_index]); */

    current_wgui_inline_color_select_item = (wgui_inline_item_color_select*) item;

    current_wgui_inline_color_select_box_spacing = (U16) spacing;

     
    if (r2lMMIFlag)
    {
        current_wgui_inline_color_select_region_x1 = (U16) (x + region_spacing);
        current_wgui_inline_color_select_region_x2 =
            (U16) (x + width - MMI_fixed_icontext_menuitem.text_x - region_spacing);
    }
    else
    {
        current_wgui_inline_color_select_region_x1 = (U16) (x + MMI_fixed_icontext_menuitem.text_x + region_spacing);
        current_wgui_inline_color_select_region_x2 = (U16) (x + width - region_spacing) - 1;     
    }

#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (IsInlineItemDefaultTextEnable())
    {
        current_wgui_inline_color_select_region_x1 = (U16) (x + region_spacing);
        current_wgui_inline_color_select_region_x2 = (U16) (x + width - region_spacing);
    }
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
    /* region_x1 and region_x2 are moved above. */
    /* current_wgui_inline_color_select_region_x1   = (U16)(x+MMI_fixed_icontext_menuitem.text_x+region_spacing); */
    current_wgui_inline_color_select_region_y1 = (U16) (y + spacing);
    /* current_wgui_inline_color_select_region_x2   = (U16)(x+width-region_spacing); */
    current_wgui_inline_color_select_region_y2 = (U16) (y + height - spacing);
     

     
    // moved above
    // gui_measure_image((PU8)GetImage(IMG_H_SELECT_LEFT), &icon_width, &icon_height);
     
    current_wgui_inline_color_select_icon1_x2 = current_wgui_inline_color_select_region_x1 - icon_spacing - 1;   
    current_wgui_inline_color_select_icon1_x1 = current_wgui_inline_color_select_icon1_x2 - icon_width + 1;
    current_wgui_inline_color_select_icon2_x1 = current_wgui_inline_color_select_region_x2 + icon_spacing + 1;   
    current_wgui_inline_color_select_icon2_x2 = current_wgui_inline_color_select_icon2_x1 + icon_width - 1;
    current_wgui_inline_color_select_icon_y = y + (height >> 1) - (icon_height >> 1);
    current_wgui_inline_color_select_icon1_pressed = 0;
    current_wgui_inline_color_select_icon2_pressed = 0;

    current_wgui_inline_color_select_box_size =
        current_wgui_inline_color_select_region_y2 - current_wgui_inline_color_select_region_y1;
    current_wgui_inline_color_select_box_count = (U16) ((current_wgui_inline_color_select_region_x2 - current_wgui_inline_color_select_region_x1 + 1 + spacing) / (current_wgui_inline_color_select_box_size + spacing));        

    current_wgui_inline_color_select_item->color_highlighted(current_wgui_inline_color_select_item->highlighted_index);

    /* dispatch remain spacing to both side */
    used_width =
        current_wgui_inline_color_select_box_size * current_wgui_inline_color_select_box_count +
        (current_wgui_inline_color_select_box_count - 1) * spacing;
    region_width = current_wgui_inline_color_select_region_x2 - current_wgui_inline_color_select_region_x1 + 1;

    current_wgui_inline_color_select_region_x1 += (U16) ((region_width - used_width) >> 1);

     
    //#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    // if(IsInlineItemDefaultTextEnable())
    current_wgui_inline_color_select_region_x2 -= (U16) ((region_width - used_width) >> 1);
    // #endif
     
    /* if total color is less then box count, adjust position */
    if (current_wgui_inline_color_select_box_count > current_wgui_inline_color_select_item->n_color)
    {
        current_wgui_inline_color_select_box_count = current_wgui_inline_color_select_item->n_color;

        /* adjust to center */
        used_width =
            current_wgui_inline_color_select_box_size * current_wgui_inline_color_select_box_count +
            (current_wgui_inline_color_select_box_count - 1) * spacing;
        region_width = current_wgui_inline_color_select_region_x2 - current_wgui_inline_color_select_region_x1 + 1;

        current_wgui_inline_color_select_region_x1 += (U16) ((region_width - used_width) >> 1);
         
    #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
        if (IsInlineItemDefaultTextEnable())
        {
            current_wgui_inline_color_select_region_x2 -= (U16) ((region_width - used_width) >> 1);
        }
    #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
         
    }

    show_highlighted_inline_color_select_menuitem();
    register_inline_color_select_menu_keys();

    clear_inline_item_keys = clear_inline_select_menu_keys;
    complete_inline_item_edit = inline_color_select_menu_change_complete;
    redraw_current_inline_item = show_highlighted_inline_color_select_menuitem;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = inline_color_select_pen_handler;
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemColorSelect
 * DESCRIPTION
 *  Set color inline selection
 * PARAMETERS
 *  item                    [IN]        Inline item pointer
 *  n_color                 [IN]        Number of color
 *  hilighted_index         [IN]        Highlighed color index
 *  list_of_color           [IN]        Array of colors
 *  color_highlighted       [IN]        Color highlighted callback function
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemColorSelect(
        InlineItem *item,
        S32 n_color,
        U16 hilighted_index,
        color_t *list_of_color,
        void (*color_highlighted) (U16 index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_color_select *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* check if still have room for another item */
    MMI_ASSERT(n_inline_color_select_items < MAX_INLINE_COLOR_SELECT_ITEMS);
    MMI_ASSERT(hilighted_index < n_color);

    item->item = (void*)&wgui_inline_color_select_items[n_inline_color_select_items];
    item->flags = INLINE_ITEM_TYPE_COLOR_SELECT;

    i = (wgui_inline_item_color_select*) item->item;

    i->flags = 0;
    i->list_of_color = list_of_color;
    i->highlighted_index = hilighted_index;
    i->n_color = (U16) n_color;
    i->color_highlighted = color_highlighted;

    item->handler = handle_inline_color_select;

    /* notify start index need re-adjust */
    i->start_index = 255;

    n_inline_color_select_items++;
}

/*----------------------------------------------------------------------------
   Inline edit general functions
----------------------------------------------------------------------------*/


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemActivation
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item            [?]         
 *  key_code        [IN]        
 *  key_event       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemActivation(InlineItem *item, S32 key_code, S32 key_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->key_code = key_code;
    item->key_event = key_event;

     
#if (0)
#endif /* (0) */ 
     
}


/*****************************************************************************
 * FUNCTION
 *  DisableInlineItem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void DisableInlineItem(InlineItem *item, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->flags |= INLINE_ITEM_DISABLED;
    wgui_inline_items[index].flags |= INLINE_ITEM_DISABLED;
}


/*****************************************************************************
 * FUNCTION
 *  EnableInlineItem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void EnableInlineItem(InlineItem *item, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->flags &= ~INLINE_ITEM_DISABLED;
    wgui_inline_items[index].flags &= ~INLINE_ITEM_DISABLED;
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemFollowUp
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemFollowUp(InlineItem *item, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->flags |= INLINE_ITEM_FOLLOW_UP;
    wgui_inline_items[index].flags |= INLINE_ITEM_FOLLOW_UP;
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemFollowDown
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemFollowDown(InlineItem *item, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->flags |= INLINE_ITEM_FOLLOW_DOWN;
    wgui_inline_items[index].flags |= INLINE_ITEM_FOLLOW_DOWN;
}


/*****************************************************************************
 * FUNCTION
 *  DisableInlineItemHighlight
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DisableInlineItemHighlight(InlineItem *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->flags |= INLINE_ITEM_DISABLE_HIGHLIGHT;
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemFullWidth
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemFullWidth(InlineItem *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->flags |= INLINE_ITEM_LEFT_ALIGN;
}


/*****************************************************************************
 * FUNCTION
 *  LeftJustifyInlineItem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void LeftJustifyInlineItem(InlineItem *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->flags &= ~INLINE_ITEM_JUSTIFY_MASK;
    item->flags |= INLINE_ITEM_LEFT_JUSTIFY;
}


/*****************************************************************************
 * FUNCTION
 *  RightJustifyInlineItem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void RightJustifyInlineItem(InlineItem *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->flags &= ~INLINE_ITEM_JUSTIFY_MASK;
     
    /* Inline items will not be shown right justify, when the default text effect
       is enabled, so removed the flag in that case. */
#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    if (!(IsInlineItemDefaultTextEnable()))
#endif 
         
        item->flags |= INLINE_ITEM_RIGHT_JUSTIFY;
}


/*****************************************************************************
 * FUNCTION
 *  CenterJustifyInlineItem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void CenterJustifyInlineItem(InlineItem *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->flags &= ~INLINE_ITEM_JUSTIFY_MASK;
    item->flags |= INLINE_ITEM_CENTER_JUSTIFY;
}


/*****************************************************************************
 * FUNCTION
 *  DisableInlineItemBoundary
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DisableInlineItemBoundary(InlineItem *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->flags &= ~INLINE_ITEM_BOUNDARY_MASK;
    item->flags |= INLINE_ITEM_BOUNDARY_NEVER;
}


/*****************************************************************************
 * FUNCTION
 *  EnableInlineItemBoundary
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void EnableInlineItemBoundary(InlineItem *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->flags &= ~INLINE_ITEM_BOUNDARY_MASK;
    item->flags |= INLINE_ITEM_BOUNDARY_ALWAYS;
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemDottedUnderline
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemDottedUnderline(InlineItem *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    item->flags |= INLINE_ITEM_DOTTED_UNDERLINE;
}


/*****************************************************************************
 * FUNCTION
 *  clear_inline_item_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void clear_inline_item_key_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((wgui_inline_item_key_code == -1) || (wgui_inline_item_key_event == -1))
    {
        return;
    }
    ClearKeyHandler((U16) wgui_inline_item_key_code, (U16) wgui_inline_item_key_event);
    wgui_inline_item_key_code = -1;
    wgui_inline_item_key_event = -1;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_item_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_item_key_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    wgui_text_menuitem_reset_scrolling();
    gdi_layer_unlock_frame_buffer();
    wgui_inline_items[wgui_inline_item_highlighted_index].handler(
                                                            wgui_inline_item_x,
                                                            wgui_inline_item_y,
                                                            wgui_inline_item_width,
                                                            wgui_inline_item_height,
                                                            wgui_inline_item_key_code,
                                                            wgui_inline_item_key_event,
                                                            &wgui_inline_items [wgui_inline_item_highlighted_index].text_p,
                                                            wgui_inline_items[wgui_inline_item_highlighted_index]. item,
                                                            wgui_inline_items[wgui_inline_item_highlighted_index]. flags,
                                                            wgui_current_history_buffer);
    wgui_current_history_buffer = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  register_inline_item_key_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void register_inline_item_key_handler(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_item_highlighted_index = index;
    wgui_inline_item_key_code = wgui_inline_items[index].key_code;
    wgui_inline_item_key_event = wgui_inline_items[index].key_event;
    if (wgui_inline_item_key_code == KEY_LSK)
    {
        SetLeftSoftkeyFunction(wgui_inline_item_key_handler, wgui_inline_item_key_event);
    }
    else if (wgui_inline_item_key_code == KEY_RSK)
    {
        SetRightSoftkeyFunction(wgui_inline_item_key_handler, wgui_inline_item_key_event);
    }
    else
    {
        SetKeyHandler(wgui_inline_item_key_handler, (U16) wgui_inline_item_key_code, (U16) wgui_inline_item_key_event);
    }
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_item_location
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x           [IN]        
 *  y           [IN]        
 *  width       [IN]        
 *  height      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_item_location(S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 item_index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_inline_item_handled)
    {
        return;
    }
    wgui_inline_item_handled = 1;
    wgui_inline_item_x = x;
    wgui_inline_item_y = y;
    wgui_inline_item_width = width;
    wgui_inline_item_height = height;
    item_index = wgui_inline_item_highlighted_index;
    if (wgui_inline_items[item_index].key_code == INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT)
    {
        gdi_layer_lock_frame_buffer();
        wgui_text_menuitem_reset_scrolling = gui_fixed_icontext_menuitem_stop_scroll;
        wgui_text_menuitem_restart_scrolling = gui_fixed_icontext_menuitem_start_scroll;
        if (((wgui_inline_items[item_index].flags & 0xff) != INLINE_ITEM_TYPE_DISPLAY_ONLY))
        {
            wgui_text_menuitem_reset_scrolling();
        }
        wgui_inline_items[item_index].handler(
                                        wgui_inline_item_x,
                                        wgui_inline_item_y,
                                        wgui_inline_item_width,
                                        wgui_inline_item_height,
                                        wgui_inline_items[item_index].key_code,
                                        wgui_inline_items[item_index].key_event,
                                        &wgui_inline_items[item_index].text_p,
                                        wgui_inline_items[item_index].item,
                                        wgui_inline_items[item_index].flags,
                                        wgui_current_history_buffer);
        wgui_current_history_buffer = NULL;
        gdi_layer_unlock_frame_buffer();
    }
    else if ((wgui_inline_items[item_index].flags & 0xff) == INLINE_ITEM_IMAGE_TEXT_TYPE)
    {
        wgui_text_menuitem_reset_scrolling();
        if (inline_image_text_edit_flag == 1)
        {
            wgui_inline_items[item_index].handler(
                                            wgui_inline_item_x,
                                            wgui_inline_item_y,
                                            wgui_inline_item_width,
                                            wgui_inline_item_height,
                                            wgui_inline_items[item_index].key_code,
                                            wgui_inline_items[item_index].key_event,
                                            &wgui_inline_items[item_index].text_p,
                                            wgui_inline_items[item_index].item,
                                            wgui_inline_items[item_index].flags,
                                            wgui_current_history_buffer);
            wgui_current_history_buffer = NULL;  
        }
        else
        {
            if ((wgui_inline_items[item_index].flags & INLINE_ITEM_DISABLE_HIGHLIGHT))
            {
                PRINT_INFORMATION(("\n DISABLE HIGHLIGHT TRUE"));
                gui_draw_filled_area(
                    wgui_inline_item_x + 17,
                    wgui_inline_item_y,
                    (wgui_inline_item_x + wgui_inline_item_width),
                    (wgui_inline_item_y + wgui_inline_item_height),
                    (wgui_inline_select_fixed_text_menuitem_theme.selected_filler));
            }
            else
            {
                PRINT_INFORMATION(("\n DISABLE HIGHLIGHT FALSE"));
                gui_draw_filled_area(
                    wgui_inline_item_x,
                    wgui_inline_item_y,
                    (wgui_inline_item_x + wgui_inline_item_width),
                    (wgui_inline_item_y + wgui_inline_item_height),
                    (wgui_inline_select_fixed_text_menuitem_theme.selected_filler));
            }
            wgui_show_image_text_item_highlight(
                wgui_inline_item_x,
                wgui_inline_item_y,
                wgui_inline_item_width,
                wgui_inline_item_height,
                &wgui_inline_items[item_index]);
            register_left_softkey_handler();
            register_inline_item_key_handler(item_index);
        }

    }
    else if ((wgui_inline_items[item_index].flags & 0xff) == INLINE_ITEM_ATTACH_IMAGE)
    {
        if ((wgui_inline_items[item_index].flags & INLINE_ITEM_DISABLE_HIGHLIGHT))
        {
            PRINT_INFORMATION(("\n DISABLE HIGHLIGHT TRUE"));
            gui_draw_filled_area(
                wgui_inline_item_x + 17,
                wgui_inline_item_y,
                (wgui_inline_item_x + wgui_inline_item_width),
                (wgui_inline_item_y + wgui_inline_item_height),
                (wgui_inline_select_fixed_text_menuitem_theme.selected_filler));
        }
        else
        {
            PRINT_INFORMATION(("\n DISABLE HIGHLIGHT FALSE"));
            gui_draw_filled_area(
                wgui_inline_item_x,
                wgui_inline_item_y,
                (wgui_inline_item_x + wgui_inline_item_width),
                (wgui_inline_item_y + wgui_inline_item_height),
                (wgui_inline_select_fixed_text_menuitem_theme.selected_filler));
        }

        wgui_show_image_attachment_item_highlight(
            wgui_inline_item_x,
            wgui_inline_item_y,
            wgui_inline_item_width,
            wgui_inline_item_height,
            &wgui_inline_items[item_index]);
        register_left_softkey_handler();
        register_inline_item_key_handler(item_index);
    }

    else
    {
        if (wgui_inline_items[item_index].flags & INLINE_ITEM_OPEN)
        {
            gdi_layer_lock_frame_buffer();
            wgui_text_menuitem_reset_scrolling = gui_fixed_icontext_menuitem_stop_scroll;
            wgui_text_menuitem_restart_scrolling = gui_fixed_icontext_menuitem_start_scroll;
            wgui_text_menuitem_reset_scrolling();
            wgui_inline_items[item_index].handler(
                                            wgui_inline_item_x,
                                            wgui_inline_item_y,
                                            wgui_inline_item_width,
                                            wgui_inline_item_height,
                                            wgui_inline_items[item_index].key_code,
                                            wgui_inline_items[item_index].key_event,
                                            &wgui_inline_items[item_index].text_p,
                                            wgui_inline_items[item_index].item,
                                            wgui_inline_items[item_index].flags,
                                            wgui_current_history_buffer);
            wgui_inline_items[item_index].flags &= ~INLINE_ITEM_OPEN;
            wgui_current_history_buffer = NULL;
            gdi_layer_unlock_frame_buffer();
        }
        else
        {
            register_left_softkey_handler();
            register_inline_item_key_handler(item_index);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_get_inline_history_size
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 wgui_get_inline_history_size(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 inline_history_size = 0, s;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (wgui_current_inline_item_type)
    {
        case INLINE_ITEM_TYPE_TEXT_EDIT:
            inline_history_size = sizeof(inline_text_edit_history);
            break;
        case INLINE_ITEM_TYPE_FULL_SCREEN_EDIT:
            inline_history_size = sizeof(inline_full_screen_edit_history);
            break;
        case INLINE_ITEM_TYPE_DATE:
            inline_history_size = sizeof(inline_date_edit_history);
            break;
        case INLINE_ITEM_TYPE_TIME:
            inline_history_size = sizeof(inline_time_edit_history);
            break;
        case INLINE_ITEM_TYPE_TIME_PERIOD:
            inline_history_size = sizeof(inline_time_period_edit_history);
            break;
        case INLINE_ITEM_TYPE_IP4_EDIT:
            inline_history_size = sizeof(inline_IP4_edit_history);
            break;
        case INLINE_ITEM_TYPE_DOW_SELECT:
            inline_history_size = sizeof(inline_DOW_select_history);
            break;
    }
    s = sizeof(list_menu_category_history);
    s = (s + 3) / 4;
    s *= 4;
    return (s + inline_history_size + sizeof(U32));
}

/* Current structure:
   history = <list menu history> + <4 bytes for flags> + <inline edit specific history>
   flags = 1: Content has been changed by user
   flags = 0: Content has not been changed by user       */


/*****************************************************************************
 * FUNCTION
 *  wgui_get_inline_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_ID          [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  
 *****************************************************************************/
U8 *wgui_get_inline_history(U16 history_ID, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 *flags;
    S32 start_offset, s;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_list_menu_category_history(history_ID, history_buffer);
    s = sizeof(list_menu_category_history);
    s = (s + 3) / 4;
    s *= 4;
    start_offset = s + sizeof(U32);
    flags = (U32*) (history_buffer + s);
    *flags = wgui_inline_list_menu_changed;
    switch (wgui_current_inline_item_type)
    {
        case INLINE_ITEM_TYPE_TEXT_EDIT:
            inline_text_edit_get_history((U8*) (history_buffer + start_offset), MMI_current_input_type);
            break;
        case INLINE_ITEM_TYPE_FULL_SCREEN_EDIT:
            inline_full_screen_edit_get_history((U8*) (history_buffer + start_offset), MMI_current_input_type);
            break;
        case INLINE_ITEM_TYPE_DATE:
            inline_date_edit_get_history((U8*) (history_buffer + start_offset));
            break;
        case INLINE_ITEM_TYPE_TIME:
            inline_time_edit_get_history((U8*) (history_buffer + start_offset));
            break;
        case INLINE_ITEM_TYPE_TIME_PERIOD:
            inline_time_period_edit_get_history((U8*) (history_buffer + start_offset));
            break;
        case INLINE_ITEM_TYPE_IP4_EDIT:
            inline_IP4_edit_get_history((U8*) (history_buffer + start_offset));
            break;
        case INLINE_ITEM_TYPE_DOW_SELECT:
            inline_DOW_select_get_history((U8*) (history_buffer + start_offset));
            break;
             
        case INLINE_ITEM_TYPE_MULTI_LINE_EDIT:
            inline_multi_line_edit_get_history((U8*) (history_buffer + start_offset), MMI_current_input_type);
            break;
             
    }
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  GetInlineDoneFlag
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 GetInlineDoneFlag(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 *flags;
    S32 s = sizeof(list_menu_category_history);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    s = (s + 3) / 4;
    s *= 4;
    if (history_buffer == NULL)
    {
        return (0);
    }
    flags = (U32*) (history_buffer + s);
    if (*flags)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineDoneFlag
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineDoneFlag(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 *flags;
    S32 s = sizeof(list_menu_category_history);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    s = (s + 3) / 4;
    s *= 4;
    if (history_buffer == NULL)
    {
        return;
    }
    flags = (U32*) (history_buffer + s);
    *flags = 1;
}


/*****************************************************************************
 * FUNCTION
 *  ResetInlineDoneFlag
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void ResetInlineDoneFlag(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 *flags;
    S32 s = sizeof(list_menu_category_history);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    s = (s + 3) / 4;
    s *= 4;
    if (history_buffer == NULL)
    {
        return;
    }
    flags = (U32*) (history_buffer + s);
    *flags = 0;
}

#define INLINE_ITEM_DATA_ID_LENGTH                    2
#define INLINE_ITEM_DATA_SIZE_LENGTH                  2

#define INLINE_ITEM_CAPTION_DATA_ID                   0x0001
#define INLINE_ITEM_DISPLAY_ONLY_DATA_ID              0x0002
#define INLINE_ITEM_USER_DEFINED_SELECT_DATA_ID          0x0003
#define INLINE_ITEM_TEXT_EDIT_DATA_ID                 0x0004
#define INLINE_ITEM_FULL_SCREEN_EDIT_DATA_ID          0x0005
#define INLINE_ITEM_SELECT_DATA_ID                    0x0006
#define INLINE_ITEM_DATE_DATA_ID                   0x0007
#define INLINE_ITEM_TIME_DATA_ID                   0x0008
#define INLINE_ITEM_TIME_PERIOD_DATA_ID                  0x0009
#define INLINE_ITEM_IP4_EDIT_DATA_ID                  0x0010
#define INLINE_ITEM_DOW_SELECT_DATA_ID                0x0011
#define INLINE_ITEM_ATTACH_IMAGE_DATA_ID              0x0012
#define INLINE_ITEM_TYPE_MULTI_LINE_DATA_ID              0x0013
#define INLINE_ITEM_COLOR_SELECT_DATA_ID              0x0014


/*****************************************************************************
 * FUNCTION
 *  wgui_get_inline_data_size
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 wgui_get_inline_data_size(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, total_size = 0, size;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < wgui_n_inline_items; i++)
    {
        size = 0;
        switch (wgui_inline_items[i].flags & 0xff)
        {
            case INLINE_ITEM_TYPE_CAPTION:
            case INLINE_ITEM_TYPE_DISPLAY_ONLY:
            case INLINE_ITEM_TYPE_USER_DEFINED_SELECT:
                break;

            case INLINE_ITEM_TYPE_TEXT_EDIT:
            {
                S32 l;
                wgui_inline_item_text_edit *ii = (wgui_inline_item_text_edit*) wgui_inline_items[i].item;

                l = ii->buffer_size * ENCODING_LENGTH;
                if (ii->input_type & INPUT_TYPE_PLUS_CHARACTER_HANDLING)
                {
                    l += ENCODING_LENGTH;
                }
                size = INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + sizeof(S32) + l;
            }
                break;

            case INLINE_ITEM_TYPE_FULL_SCREEN_EDIT:
            {
                S32 l;
                wgui_inline_item_text_edit *ii = (wgui_inline_item_text_edit*) wgui_inline_items[i].item;

                l = ii->buffer_size * ENCODING_LENGTH;
                if (ii->input_type & INPUT_TYPE_PLUS_CHARACTER_HANDLING)
                {
                    l += ENCODING_LENGTH;
                }
                size = INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + sizeof(U32) + sizeof(S32) + (l << 1);
            }
                break;

            case INLINE_ITEM_TYPE_SELECT:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + 2;
            }
                break;

 
            case INLINE_ITEM_ATTACH_IMAGE:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + 2;
            }
                break;
            case INLINE_ITEM_TYPE_DATE:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + 5 * ENCODING_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH;
            }
                break;

            case INLINE_ITEM_TYPE_TIME:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH;
            }
                break;

            case INLINE_ITEM_TYPE_TIME_PERIOD:
            {
                size =
                    INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH +
                    3 * ENCODING_LENGTH;
            }
                break;

            case INLINE_ITEM_TYPE_IP4_EDIT:
            {
                size =
                    INLINE_ITEM_DATA_ID_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH +
                    4 * ENCODING_LENGTH;
            }
                break;

            case INLINE_ITEM_TYPE_DOW_SELECT:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + 7;
            }
                break;
        #if defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__) /* 4/5 */
            case INLINE_ITEM_TYPE_MULTI_LINE_EDIT:
            case INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + sizeof(multiline_inputbox_category_history);
            }
                break;
        #endif /* defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__) */ 

            case INLINE_ITEM_TYPE_COLOR_SELECT:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + 4;
            }
                break;
        }
        total_size += size;
    }
    return (total_size);
}



/*****************************************************************************
 * FUNCTION
 *  get_inline_item_text_edit_offset
 * DESCRIPTION
 *  
 * PARAMETERS
 *  list_of_items       [?]         
 *  last_item           [IN]        
 *  flag                [IN]        
 * RETURNS
 *  
 *****************************************************************************/
S32 get_inline_item_text_edit_offset(InlineItem *list_of_items, S32 last_item, U8 flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, total_size = 0, size;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i <= last_item - flag; i++)
    {
        size = 0;
        switch (list_of_items[i].flags & 0xff)
        {
            case INLINE_ITEM_TYPE_CAPTION:
            case INLINE_ITEM_TYPE_DISPLAY_ONLY:
            case INLINE_ITEM_TYPE_USER_DEFINED_SELECT:
                break;

            case INLINE_ITEM_TYPE_TEXT_EDIT:
            {
                S32 l;
                wgui_inline_item_text_edit *ii = (wgui_inline_item_text_edit*) list_of_items[i].item;

                l = ii->buffer_size * ENCODING_LENGTH;
                if (ii->input_type & INPUT_TYPE_PLUS_CHARACTER_HANDLING)
                {
                    l += ENCODING_LENGTH;
                }
                if (i == last_item)
                {
                    size = INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + sizeof(S32);
                }
                else
                {
                    size = INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + sizeof(S32) + l;
                }
            }
                break;

            case INLINE_ITEM_TYPE_FULL_SCREEN_EDIT:
            {
                S32 l;
                wgui_inline_item_text_edit *ii = (wgui_inline_item_text_edit*) list_of_items[i].item;

                l = ii->buffer_size * ENCODING_LENGTH;
                if (ii->input_type & INPUT_TYPE_PLUS_CHARACTER_HANDLING)
                {
                    l += ENCODING_LENGTH;
                }
                size = INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + sizeof(U32) + sizeof(S32) + (l << 1);
            }
                break;

            case INLINE_ITEM_TYPE_SELECT:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + 2;
            }
                break;
            case INLINE_ITEM_ATTACH_IMAGE:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + 2;
            }
                break;
            case INLINE_ITEM_TYPE_DATE:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + 5 * ENCODING_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH;
            }
                break;

            case INLINE_ITEM_TYPE_TIME:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH;
            }
                break;

            case INLINE_ITEM_TYPE_TIME_PERIOD:
            {
                size =
                    INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH +
                    3 * ENCODING_LENGTH;
            }
                break;

            case INLINE_ITEM_TYPE_IP4_EDIT:
            {
                size =
                    INLINE_ITEM_DATA_ID_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH +
                    4 * ENCODING_LENGTH;
            }
                break;

            case INLINE_ITEM_TYPE_DOW_SELECT:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + 7;
            }
                break;
        #if defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__)
            case INLINE_ITEM_TYPE_MULTI_LINE_EDIT:
            case INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + sizeof(multiline_inputbox_category_history);
            }
                break;
        #endif /* defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__) */ 
            case INLINE_ITEM_TYPE_COLOR_SELECT:
            {
                size = INLINE_ITEM_DATA_ID_LENGTH + 4;
            }
                break;
        }
        total_size += size;
    }
    return (total_size);
}



/*****************************************************************************
 * FUNCTION
 *  AppendInlineItemTextEditString
 * DESCRIPTION
 *  
 * PARAMETERS
 *  input_type          [IN]        
 *  buffer              [?]         
 *  list_of_items       [?]         
 *  number_of_items     [IN]        
 *  size                [?]         
 *  history             [?]         
 *  plus_handling       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
S32 AppendInlineItemTextEditString(
        S16 input_type,
        U8 *buffer,
        InlineItem *list_of_items,
        S32 number_of_items,
        S32 *size,
        history_t *history,
        U8 plus_handling)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *gui_buffer;
    U8 *input_buffer;
    U8 *data_buffer;
    S32 AppendLen = 0;

    /* U32 *flags; */
    S32 s;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* byte list_menu_changed=1; */
    gui_buffer = history->guiBuffer;
    input_buffer = history->inputBuffer;

    if (!set_list_menu_category_history(MMI_CATEGORY57_ID, gui_buffer))
    {
        return -1;
    }
    if (MMI_fixed_list_menu.highlighted_item > number_of_items)
    {
        return -1;
    }
    s = sizeof(list_menu_category_history);
    s = (s + 3) / 4;
    s *= 4;
 
    set_wgui_inline_list_menu_changed();
    memcpy(gui_buffer + s, &wgui_inline_list_menu_changed, sizeof(U32));
  
    inline_text_edit_set_history(INLINE_TEXT_EDIT_HISTORY_ID, (gui_buffer + s + sizeof(U32)), &input_type);
    data_buffer =
        input_buffer + get_inline_item_text_edit_offset(list_of_items, MMI_fixed_list_menu.highlighted_item, 0);
    /* For Append String */
    if (plus_handling == 1)
    {
        if (MMI_inline_singleline_inputbox.text_length == 2)
        {
            if (buffer[0] != '+')
            {
                plus_handling = 0;
            }
        }
        else
        {
            if (data_buffer[0] != '+')
            {
                plus_handling = 0;
            }
        }
    }
    else
    {
        plus_handling = 0;
    }

    if ((MMI_inline_singleline_inputbox.allocated_length - MMI_inline_singleline_inputbox.text_length) >=
        (pfnUnicodeStrlen((PS8) buffer) - plus_handling) * ENCODING_LENGTH)
    {
        memcpy(
            data_buffer + MMI_inline_singleline_inputbox.text_length - ENCODING_LENGTH,
            buffer,
            (pfnUnicodeStrlen((PS8) buffer) + 1) * ENCODING_LENGTH);
	AppendLen = (pfnUnicodeStrlen((PS8) buffer) - plus_handling)*ENCODING_LENGTH;
    }
    else
    {
        memcpy(
            data_buffer + MMI_inline_singleline_inputbox.text_length - ENCODING_LENGTH,
            buffer,
            MMI_inline_singleline_inputbox.allocated_length - MMI_inline_singleline_inputbox.text_length - ENCODING_LENGTH);
		#if 0
        memcpy(data_buffer + MMI_inline_singleline_inputbox.allocated_length - ENCODING_LENGTH, 0, ENCODING_LENGTH);
		#else
		memset(data_buffer + MMI_inline_singleline_inputbox.allocated_length - ENCODING_LENGTH, 0, ENCODING_LENGTH);
		#endif
	AppendLen = MMI_inline_singleline_inputbox.allocated_length - MMI_inline_singleline_inputbox.text_length - ENCODING_LENGTH;
    }
    /*
     * For Set String
     * if(MMI_inline_singleline_inputbox.allocated_length-ENCODING_LENGTH>pfnUnicodeStrlen((PS8)buffer)*ENCODING_LENGTH)
     * {
     * memcpy(data_buffer, buffer, (pfnUnicodeStrlen((PS8)buffer)+1)*ENCODING_LENGTH);
     * }
     * else
     * {
     * memcpy(data_buffer,buffer,MMI_inline_singleline_inputbox.allocated_length-ENCODING_LENGTH);
     * memcpy(data_buffer+MMI_inline_singleline_inputbox.allocated_length-ENCODING_LENGTH,'\0',ENCODING_LENGTH);
     * }
     */
    MMI_inline_singleline_inputbox.text_length = pfnUnicodeStrlen((PS8) data_buffer) * ENCODING_LENGTH + ENCODING_LENGTH;
    MMI_inline_singleline_inputbox.text = data_buffer;
    MMI_inline_singleline_inputbox.current_text_p = MMI_inline_singleline_inputbox.text + AppendLen;        /* (UI_buffer_type)(MMI_inline_singleline_inputbox.text+(MMI_inline_singleline_inputbox.text_length-ENCODING_LENGTH)); */
	gui_single_line_input_box_locate_cursor(&MMI_inline_singleline_inputbox);
    inline_text_edit_get_history((U8*) (gui_buffer + s + sizeof(U32)), MMI_current_input_type);
    *size = get_inline_item_text_edit_offset(list_of_items, number_of_items, 1);
    return 0;
}


/*****************************************************************************
 * FUNCTION
 *  wgui_get_inline_data
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 *wgui_get_inline_data(U8 *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, offset = 0, size;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < wgui_n_inline_items; i++)
    {
        size = 0;
        switch (wgui_inline_items[i].flags & 0xff)
        {
            case INLINE_ITEM_TYPE_CAPTION:
            case INLINE_ITEM_TYPE_DISPLAY_ONLY:
            case INLINE_ITEM_TYPE_USER_DEFINED_SELECT:
                break;

            case INLINE_ITEM_TYPE_TEXT_EDIT:
            {
                S32 l;
                U16 ID = INLINE_ITEM_TEXT_EDIT_DATA_ID;
                wgui_inline_item_text_edit *ii = (wgui_inline_item_text_edit*) wgui_inline_items[i].item;

                l = ii->buffer_size * ENCODING_LENGTH;
                if (ii->input_type & INPUT_TYPE_PLUS_CHARACTER_HANDLING)
                {
                    l += ENCODING_LENGTH;
                }
                size = INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + sizeof(S32) + l;
                gui_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                gui_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (U8*) & l,
                    INLINE_ITEM_DATA_SIZE_LENGTH);
                gui_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH],
                    (U8*) & (ii->UCS2_count),
                    sizeof(S32));
                gui_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + sizeof(S32)],
                    (U8*) ii->buffer,
                    l);
            }
                break;

            case INLINE_ITEM_TYPE_FULL_SCREEN_EDIT:
            {
                S32 l;
                U16 ID = INLINE_ITEM_FULL_SCREEN_EDIT_DATA_ID;
                wgui_inline_item_text_edit *ii = (wgui_inline_item_text_edit*) wgui_inline_items[i].item;
                U32 flags = 0;
                S32 flags_size = sizeof(U32);

                flags = (U32) (wgui_inline_items[i].flags >> 8);
                l = ii->buffer_size * ENCODING_LENGTH;
                if (ii->input_type & INPUT_TYPE_PLUS_CHARACTER_HANDLING)
                {
                    l += ENCODING_LENGTH;
                }
                size = INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + flags_size + (l << 1) + sizeof(S32);
                gui_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                gui_memcpy((U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH], (U8*) & flags, flags_size);
                gui_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + flags_size],
                    (U8*) & l,
                    INLINE_ITEM_DATA_SIZE_LENGTH);
                gui_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + flags_size + INLINE_ITEM_DATA_SIZE_LENGTH],
                    (U8*) & (ii->UCS2_count),
                    sizeof(S32));
                gui_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + flags_size + sizeof(S32)],
                    (U8*) wgui_inline_full_screen_text_edit_buffer,
                    l);
                gui_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + flags_size + sizeof(S32) + l],
                    (U8*) ii->buffer,
                    l);
            }
                break;

            case INLINE_ITEM_TYPE_SELECT:
            {
                U16 ID = INLINE_ITEM_SELECT_DATA_ID;
                S32 highlighted_item;
                wgui_inline_item_select *ii = (wgui_inline_item_select*) wgui_inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + 2;
                highlighted_item = *ii->highlighted_item;
                gui_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                gui_memcpy((U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH], (U8*) & highlighted_item, 2);
            }
                break;

 
            case INLINE_ITEM_ATTACH_IMAGE:
            {
                U16 ID = INLINE_ITEM_ATTACH_IMAGE_DATA_ID;
                S32 higlighted_item;
                wgui_inline_image_attachment *ii = (wgui_inline_image_attachment*) wgui_inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + 2;
                higlighted_item = ii->highlited_image;
                gui_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                gui_memcpy((U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH], (U8*) & higlighted_item, 2);
            }
                break;

            case INLINE_ITEM_TYPE_DATE:
            {
                U16 ID = INLINE_ITEM_DATE_DATA_ID;
                wgui_inline_item_date *ii = (wgui_inline_item_date*) wgui_inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + 5 * ENCODING_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH;
                gui_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                gui_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (UI_string_type) ii->year_buffer);
                gui_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 5 * ENCODING_LENGTH],
                    (UI_string_type) ii->month_buffer);
                gui_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 5 * ENCODING_LENGTH + 3 * ENCODING_LENGTH],
                    (UI_string_type) ii->day_buffer);
            }
                break;

            case INLINE_ITEM_TYPE_TIME:
            {
                U16 ID = INLINE_ITEM_TIME_DATA_ID;
                wgui_inline_item_time *ii = (wgui_inline_item_time*) wgui_inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH;
                gui_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                gui_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (UI_string_type) ii->hours_buffer);
                gui_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH],
                    (UI_string_type) ii->minutes_buffer);
            }
                break;

            case INLINE_ITEM_TYPE_TIME_PERIOD:
            {
                U16 ID = INLINE_ITEM_TIME_PERIOD_DATA_ID;
                wgui_inline_item_time_period *ii = (wgui_inline_item_time_period*) wgui_inline_items[i].item;

                size =
                    INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH +
                    3 * ENCODING_LENGTH;
                gui_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                gui_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (UI_string_type) ii->hours_buffer1);
                gui_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH],
                    (UI_string_type) ii->minutes_buffer1);
                gui_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH],
                    (UI_string_type) ii->hours_buffer2);
                gui_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH],
                    (UI_string_type) ii->minutes_buffer2);
            }
                break;

            case INLINE_ITEM_TYPE_IP4_EDIT:
            {
                U16 ID = INLINE_ITEM_IP4_EDIT_DATA_ID;
                wgui_inline_item_IP4 *ii = (wgui_inline_item_IP4*) wgui_inline_items[i].item;

                size =
                    INLINE_ITEM_DATA_ID_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH +
                    4 * ENCODING_LENGTH;
                gui_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                gui_strcpy((UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH], (UI_string_type) ii->b1);
                gui_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 4 * ENCODING_LENGTH],
                    (UI_string_type) ii->b2);
                gui_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH],
                    (UI_string_type) ii->b3);
                gui_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH],
                    (UI_string_type) ii->b4);
            }
                break;

            case INLINE_ITEM_TYPE_DOW_SELECT:
            {
                U16 ID = INLINE_ITEM_DOW_SELECT_DATA_ID;
                wgui_inline_item_DOW_select *ii = (wgui_inline_item_DOW_select*) wgui_inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + 7;
                gui_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                gui_memcpy((U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH], (U8*) ii->list_of_states, 7);
            }
                break;
        #if defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__) /* 4/5 */
            case INLINE_ITEM_TYPE_MULTI_LINE_EDIT:
            case INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY:
            {
                /* U16 ID=INLINE_ITEM_TYPE_MULTI_LINE_DATA_ID; */
                size = INLINE_ITEM_DATA_ID_LENGTH + sizeof(multiline_inputbox_category_history);
                get_multiline_inputbox_category_history(
                    MMI_INLINE_BOX_HISTORY_ID,
                    &data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    MMI_current_input_type);
            }
                break;
        #endif /* defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__) */ 
            case INLINE_ITEM_TYPE_COLOR_SELECT:
            {
                U16 ID = INLINE_ITEM_COLOR_SELECT_DATA_ID;
                S32 highlighted_index;
                S32 start_index;

                wgui_inline_item_color_select *ii = (wgui_inline_item_color_select*) wgui_inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + 4;

                highlighted_index = ii->highlighted_index;
                start_index = ii->start_index;

                gui_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                gui_memcpy((U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH], (U8*) & highlighted_index, 2);
                gui_memcpy((U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 2], (U8*) & start_index, 2);
            }
                break;
        }
        offset += size;
    }
    return (data);
}

#define reverse_coolsand_UI_memcpy(b1,b2,n) gui_memcpy((b2),(b1),(n))
#define reverse_coolsand_UI_strcpy(s1,s2)      gui_strcpy(s2,s1)


/*****************************************************************************
 * FUNCTION
 *  wgui_set_inline_data
 * DESCRIPTION
 *  
 * PARAMETERS
 *  inline_items        [?]         
 *  n_items             [IN]        
 *  data                [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_set_inline_data(wgui_inline_item *inline_items, S32 n_items, U8 *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, offset = 0, size;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (data == NULL)
    {
        return;
    }
    for (i = 0; i < n_items; i++)
    {
        size = 0;
        switch (inline_items[i].flags & 0xff)
        {
            case INLINE_ITEM_TYPE_CAPTION:
            case INLINE_ITEM_TYPE_DISPLAY_ONLY:
            case INLINE_ITEM_TYPE_USER_DEFINED_SELECT:
                break;

            case INLINE_ITEM_TYPE_TEXT_EDIT:
            {
                S32 l;
                U16 ID = INLINE_ITEM_TEXT_EDIT_DATA_ID;
                wgui_inline_item_text_edit *ii = (wgui_inline_item_text_edit*) inline_items[i].item;

                l = ii->buffer_size * ENCODING_LENGTH;
                if (ii->input_type & INPUT_TYPE_PLUS_CHARACTER_HANDLING)
                {
                    l += ENCODING_LENGTH;
                }
                size = INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + sizeof(S32) + l;
                reverse_coolsand_UI_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (U8*) & l,
                    INLINE_ITEM_DATA_SIZE_LENGTH);
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH],
                    (U8*) & (ii->UCS2_count),
                    sizeof(S32));
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + sizeof(S32)],
                    (U8*) ii->buffer,
                    l);
            }
                break;

            case INLINE_ITEM_TYPE_FULL_SCREEN_EDIT:
            {
                S32 l;
                U16 ID = INLINE_ITEM_FULL_SCREEN_EDIT_DATA_ID;
                U32 flags;
                S32 flags_size = sizeof(U32);
                wgui_inline_item_text_edit *ii = (wgui_inline_item_text_edit*) inline_items[i].item;

                l = ii->buffer_size * ENCODING_LENGTH;
                if (ii->input_type & INPUT_TYPE_PLUS_CHARACTER_HANDLING)
                {
                    l += ENCODING_LENGTH;
                }
                size = INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + flags_size + (l << 1) + sizeof(S32);
                reverse_coolsand_UI_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (U8*) & flags,
                    flags_size);
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + flags_size],
                    (U8*) & l,
                    INLINE_ITEM_DATA_SIZE_LENGTH);
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + flags_size + INLINE_ITEM_DATA_SIZE_LENGTH],
                    (U8*) & (ii->UCS2_count),
                    sizeof(S32));
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + flags_size + sizeof(S32)],
                    (U8*) wgui_inline_full_screen_text_edit_buffer,
                    l);
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + INLINE_ITEM_DATA_SIZE_LENGTH + flags_size + sizeof(S32) + l],
                    (U8*) ii->buffer,
                    l);
                inline_items[i].flags &= 0x000000ff;
                inline_items[i].flags |= flags << 8;
                ii->flags |= (inline_items[i].flags & 0xffffff00);
            }
                break;

            case INLINE_ITEM_TYPE_SELECT:
            {
                U16 ID = INLINE_ITEM_SELECT_DATA_ID;
                S32 highlighted_item = 0;
                wgui_inline_item_select *ii = (wgui_inline_item_select*) inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + 2;
                reverse_coolsand_UI_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (U8*) & highlighted_item,
                    2);
                *ii->highlighted_item = highlighted_item;
                if ((highlighted_item >= 0) && (highlighted_item < ii->n_items))
                {
                    inline_items[i].text_p = ii->list_of_strings[highlighted_item];
                }
            }
                break;

 
            case INLINE_ITEM_ATTACH_IMAGE:
            {
                U16 ID = INLINE_ITEM_ATTACH_IMAGE_DATA_ID;
                S32 highlighted_item = 0;
                //wgui_inline_image_attachment *ii = (wgui_inline_image_attachment*) inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + 2;
                reverse_coolsand_UI_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (U8*) & highlighted_item,
                    2);
                /* do not apply history, application will set index */
                //ii->highlited_image = (S16) highlighted_item;
            }
                break;

            case INLINE_ITEM_TYPE_DATE:
            {
                U16 ID = INLINE_ITEM_DATE_DATA_ID;
                wgui_inline_item_date *ii = (wgui_inline_item_date*) inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + 5 * ENCODING_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH;
                reverse_coolsand_UI_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (UI_string_type) ii->year_buffer);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 5 * ENCODING_LENGTH],
                    (UI_string_type) ii->month_buffer);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 5 * ENCODING_LENGTH + 3 * ENCODING_LENGTH],
                    (UI_string_type) ii->day_buffer);
                ii->string_construct_callback(
                        inline_items[i].text_p,
                        ii->day_buffer,
                        ii->month_buffer,
                        ii->year_buffer);
            }
                break;

            case INLINE_ITEM_TYPE_TIME:
            {
                U16 ID = INLINE_ITEM_TIME_DATA_ID;
                wgui_inline_item_time *ii = (wgui_inline_item_time*) inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH;
                reverse_coolsand_UI_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (UI_string_type) ii->hours_buffer);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH],
                    (UI_string_type) ii->minutes_buffer);
                ii->string_construct_callback(
                        inline_items[i].text_p,
                        ii->hours_buffer,
                        ii->minutes_buffer,
                        ii->AM_PM_flag);
            }
                break;

            case INLINE_ITEM_TYPE_TIME_PERIOD:
            {
                U16 ID = INLINE_ITEM_TIME_PERIOD_DATA_ID;
                wgui_inline_item_time_period *ii = (wgui_inline_item_time_period*) inline_items[i].item;

                size =
                    INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH +
                    3 * ENCODING_LENGTH;
                reverse_coolsand_UI_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (UI_string_type) ii->hours_buffer1);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH],
                    (UI_string_type) ii->minutes_buffer1);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH],
                    (UI_string_type) ii->hours_buffer2);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH + 3 * ENCODING_LENGTH],
                    (UI_string_type) ii->minutes_buffer2);
                ii->string_construct_callback(
                        inline_items[i].text_p,
                        ii->hours_buffer1,
                        ii->minutes_buffer1,
                        ii->AM_PM_flag1,
                        ii->hours_buffer2,
                        ii->minutes_buffer2,
                        ii->AM_PM_flag2);
            }
                break;

            case INLINE_ITEM_TYPE_IP4_EDIT:
            {
                U16 ID = INLINE_ITEM_IP4_EDIT_DATA_ID;
                wgui_inline_item_IP4 *ii = (wgui_inline_item_IP4*) inline_items[i].item;

                size =
                    INLINE_ITEM_DATA_ID_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH +
                    4 * ENCODING_LENGTH;
                reverse_coolsand_UI_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (UI_string_type) ii->b1);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 4 * ENCODING_LENGTH],
                    (UI_string_type) ii->b2);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH],
                    (UI_string_type) ii->b3);
                reverse_coolsand_UI_strcpy(
                    (UI_string_type) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH + 4 * ENCODING_LENGTH],
                    (UI_string_type) ii->b4);
                ii->string_construct_callback(inline_items[i].text_p, ii->b1, ii->b2, ii->b3, ii->b4);
            }
                break;

            case INLINE_ITEM_TYPE_DOW_SELECT:
            {
                U16 ID = INLINE_ITEM_DOW_SELECT_DATA_ID;
                wgui_inline_item_DOW_select *ii = (wgui_inline_item_DOW_select*) inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + 7;
                reverse_coolsand_UI_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (U8*) ii->list_of_states,
                    7);
            }
                break;
        #if defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__) /* 4/5 */
            case INLINE_ITEM_TYPE_MULTI_LINE_EDIT:
            case INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY:
            {
                 
                wgui_inline_item_text_edit *ii = (wgui_inline_item_text_edit*) inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + sizeof(multiline_inputbox_category_history);
                set_multiline_inputbox_category_history(
                    MMI_INLINE_BOX_HISTORY_ID,
                    &data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (S16*) & MMI_current_input_type);
                ii->input_type = MMI_current_input_type;
                flag_use_current_input_type = TRUE;
                 
            }
                break;
        #endif /* defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__) */ 
            case INLINE_ITEM_TYPE_COLOR_SELECT:
            {
                U16 ID = INLINE_ITEM_SELECT_DATA_ID;
                S32 highlighted_index = 0;
                S32 start_index = 0;

                wgui_inline_item_color_select *ii = (wgui_inline_item_color_select*) inline_items[i].item;

                size = INLINE_ITEM_DATA_ID_LENGTH + 4;

                reverse_coolsand_UI_memcpy((U8*) & data[offset], (U8*) & ID, INLINE_ITEM_DATA_ID_LENGTH);
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH],
                    (U8*) & highlighted_index,
                    2);
                reverse_coolsand_UI_memcpy(
                    (U8*) & data[offset + INLINE_ITEM_DATA_ID_LENGTH + 2],
                    (U8*) & start_index,
                    2);

                ii->highlighted_index = (U16) (highlighted_index);
                ii->start_index = (U16) (start_index);
            }
                break;

        }
        offset += size;
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_update_inline_data
 * DESCRIPTION
 *  To update inline buffer to user buffer before complete the inline item
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_update_inline_data(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < wgui_n_inline_items; i++)
    {
        switch (wgui_inline_items[i].flags & 0xff)
        {
            case INLINE_ITEM_TYPE_CAPTION:
            case INLINE_ITEM_TYPE_DISPLAY_ONLY:
            case INLINE_ITEM_TYPE_USER_DEFINED_SELECT:
                break;

            case INLINE_ITEM_TYPE_TEXT_EDIT:
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_text_edit_item->buffer,
                    (UI_string_type) wgui_inline_text_edit_buffer);
                break;

            case INLINE_ITEM_TYPE_FULL_SCREEN_EDIT:
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_text_edit_item->buffer,
                    (UI_string_type) wgui_inline_full_screen_text_edit_buffer);
                break;

            case INLINE_ITEM_TYPE_SELECT:
                break;

            case INLINE_ITEM_ATTACH_IMAGE:
                break;

            case INLINE_ITEM_TYPE_DATE:
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_date_item->day_buffer,
                    (UI_string_type) wgui_inline_date_edit_day_buffer);
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_date_item->month_buffer,
                    (UI_string_type) wgui_inline_date_edit_month_buffer);
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_date_item->year_buffer,
                    (UI_string_type) wgui_inline_date_edit_year_buffer);
                break;

            case INLINE_ITEM_TYPE_TIME:
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_time_item->hours_buffer,
                    (UI_string_type) wgui_inline_time_edit_hours_buffer);
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_time_item->minutes_buffer,
                    (UI_string_type) wgui_inline_time_edit_minutes_buffer);
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_time_item->seconds_buffer,
                    (UI_string_type) wgui_inline_time_edit_seconds_buffer);
                break;

            case INLINE_ITEM_TYPE_TIME_PERIOD:
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_time_period_item->hours_buffer1,
                    (UI_string_type) wgui_inline_time_period_edit_hours_buffer1);
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_time_period_item->minutes_buffer1,
                    (UI_string_type) wgui_inline_time_period_edit_minutes_buffer1);
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_time_period_item->hours_buffer2,
                    (UI_string_type) wgui_inline_time_period_edit_hours_buffer2);
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_time_period_item->minutes_buffer2,
                    (UI_string_type) wgui_inline_time_period_edit_minutes_buffer2);
                break;

            case INLINE_ITEM_TYPE_IP4_EDIT:
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_IP4_item->b1,
                    (UI_string_type) wgui_inline_IP4_edit_buffer1);
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_IP4_item->b2,
                    (UI_string_type) wgui_inline_IP4_edit_buffer2);
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_IP4_item->b3,
                    (UI_string_type) wgui_inline_IP4_edit_buffer3);
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_IP4_item->b4,
                    (UI_string_type) wgui_inline_IP4_edit_buffer4);
                break;

            case INLINE_ITEM_TYPE_DOW_SELECT:
                break;

        #if defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__)
            case INLINE_ITEM_TYPE_MULTI_LINE_EDIT:
                gui_strcpy(
                    (UI_string_type) current_wgui_inline_text_edit_item->buffer,
                    (UI_string_type) wgui_inline_full_screen_text_edit_buffer);
                break;

            case INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY:
                break;
        #endif /* defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__) */ 
            case INLINE_ITEM_TYPE_COLOR_SELECT:
                break;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  GetInlineEditUCS2Count
 * DESCRIPTION
 *  
 * PARAMETERS
 *  i       [?]     
 * RETURNS
 *  
 *****************************************************************************/
S32 GetInlineEditUCS2Count(wgui_inline_item *i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 r = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (i->flags & 0xff)
    {
        case INLINE_ITEM_TYPE_TEXT_EDIT:
        case INLINE_ITEM_TYPE_FULL_SCREEN_EDIT:
        {
            wgui_inline_item_text_edit *p = (wgui_inline_item_text_edit*) i->item;

            r = p->UCS2_count;
        }
            break;
    }
    return (r);
}

/*----------------------------------------------------------------------------
   Default unhighlight handler used for inline editing menu
----------------------------------------------------------------------------*/


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_edit_default_list_unhighlight_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item_index      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_edit_default_list_unhighlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    wgui_current_inline_item_type = 0;
    if (item_index > 0 &&
        ((wgui_inline_items[item_index].flags & INLINE_ITEM_TYPE_MULTI_LINE_EDIT) == INLINE_ITEM_TYPE_MULTI_LINE_EDIT))
    {
        reset_multiline_inputbox_vertical_scroll();
    }
}

/*----------------------------------------------------------------------------
   Default highlight handler used for inline editing menu
----------------------------------------------------------------------------*/


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_edit_default_list_highlight_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item_index      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_inline_edit_default_list_highlight_handler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_menu_shortcut_number = item_index + 1;
    MMI_highlighted_item_text = get_item_text(item_index);
    wgui_inline_item_handled = 0;
    wgui_inline_item_highlighted_index = item_index;
    register_default_hide_softkeys();
    complete_inline_item_edit = UI_dummy_function;
    current_wgui_inline_item = &wgui_inline_items[item_index];
    if (wgui_inline_items[item_index].flags & INLINE_ITEM_DISABLED)
    {
        if (wgui_inline_edit_arrow_key_flag == CATEGORY57_UP_ARROW_KEY_FLAG)
        {
            gui_fixed_list_menu_goto_previous_item(&MMI_fixed_list_menu);
        }
        else if (wgui_inline_edit_arrow_key_flag == CATEGORY57_DOWN_ARROW_KEY_FLAG)
        {
            gui_fixed_list_menu_goto_next_item(&MMI_fixed_list_menu);
        }
        else
        {
            gui_fixed_list_menu_goto_next_item(&MMI_fixed_list_menu);
        }
        return;
    }
    else if (wgui_inline_items[item_index].flags & INLINE_ITEM_FOLLOW_DOWN)
    {
        void (*saved_highlight_handler) (S32) = MMI_fixed_list_menu.item_highlighted;
        S32 display_position = MMI_fixed_list_menu.highlighted_item - MMI_fixed_list_menu.first_displayed_item + 1;

         
        /* correct the check so that the menuitem will be displayed which is just below
           (follow down) the current menuitem, when the display position of current
           menuitem is greater than equal to the no of displayed menuitems. */
        /* if(display_position==MMI_fixed_list_menu.displayed_items) */
        if (display_position >= MMI_fixed_list_menu.displayed_items)
             
        {
            MMI_fixed_list_menu.item_highlighted = UI_dummy_function_S32 ;
            gui_fixed_list_menu_goto_next_item(&MMI_fixed_list_menu);
            gui_fixed_list_menu_goto_previous_item(&MMI_fixed_list_menu);
            MMI_fixed_list_menu.item_highlighted = saved_highlight_handler;
        }
    }
    else if (wgui_inline_items[item_index].flags & INLINE_ITEM_FOLLOW_UP)
    {
        void (*saved_highlight_handler) (S32) = MMI_fixed_list_menu.item_highlighted;

        if (MMI_fixed_list_menu.highlighted_item == MMI_fixed_list_menu.first_displayed_item)
        {
            MMI_fixed_list_menu.item_highlighted = UI_dummy_function_S32 ;
            gui_fixed_list_menu_goto_previous_item(&MMI_fixed_list_menu);
            gui_fixed_list_menu_goto_next_item(&MMI_fixed_list_menu);
            MMI_fixed_list_menu.item_highlighted = saved_highlight_handler;
        }
    }
    /* Change the softkey functions and labels   */
#if defined(__MMI_TOUCH_SCREEN__)
     
    if (!mmi_pen_editor_is_closecategory57 && mmi_pen_editor_get_list_menu_height())
    {
        wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
        resize_fixed_list(MMI_content_width, mmi_pen_editor_get_list_menu_height());
    }
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 

    switch (wgui_inline_items[item_index].flags & 0xff)
    {
        case INLINE_ITEM_TYPE_CAPTION:
        {
            if (wgui_inline_edit_arrow_key_flag == CATEGORY57_UP_ARROW_KEY_FLAG)
            {
                gui_fixed_list_menu_goto_previous_item(&MMI_fixed_list_menu);
            }
            else if (wgui_inline_edit_arrow_key_flag == CATEGORY57_DOWN_ARROW_KEY_FLAG)
            {
                gui_fixed_list_menu_goto_next_item(&MMI_fixed_list_menu);
            }
            else
            {
                gui_fixed_list_menu_goto_next_item(&MMI_fixed_list_menu);
            }
            return;
        }

        case INLINE_ITEM_TYPE_DISPLAY_ONLY:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
            clear_left_softkey_handler();
            set_left_softkey_label(NULL);
            set_left_softkey_icon(NULL);
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        }
            break;
        case INLINE_ITEM_TYPE_TEXT_EDIT:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
            set_left_softkey_label(get_string(STR_GLOBAL_OK));
            set_left_softkey_icon(NULL);
            register_left_softkey_handler();
            /* Modified for LSK calling "Back" or "Done" functions based on dirty flag          */
            /* set_left_softkey_function(execute_wgui_inline_edit_DONE_function,KEY_EVENT_UP);     */
            /* set_left_softkey_function(execute_wgui_inline_edit_RSK_function,KEY_EVENT_UP); */
             
            if (wgui_inline_list_menu_disable_done)
            {
                set_left_softkey_function(execute_wgui_inline_edit_DONE_function, KEY_EVENT_UP);
            #if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
                mmi_pen_editor_set_inputbox_LSK_Function(execute_wgui_inline_edit_DONE_function);
            #endif 
            }
            else
            {
                set_left_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
            #if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
                mmi_pen_editor_set_inputbox_LSK_Function(execute_wgui_inline_edit_RSK_function);
            #endif 
            }
        #if defined(__MMI_TOUCH_SCREEN__)
             
            if (!mmi_pen_editor_is_closecategory57)
            {
                 
                inline_text_edit_history *h = (inline_text_edit_history*) wgui_current_history_buffer;

                if (wgui_current_history_buffer && history_valid(h->history_ID, INLINE_TEXT_EDIT_HISTORY_ID))
                {
                    wgui_setup_virtual_keyboard(h->vk_lang_type);
                }
                else
                {
                    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) wgui_inline_items[item_index].item;
                    U16 input_type_only = i->input_type & INPUT_TYPE_MASK;

                    switch (input_type_only)
                    {
                        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE:
                        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE:
                        default:
                            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_TRAY);
                            break;
                        case INPUT_TYPE_PHONE_NUMBER:
				MMI_current_input_type = INPUT_TYPE_PHONE_NUMBER;//Renwy add for bug 8779 2008/06/28
                            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
                            break;
                        case INPUT_TYPE_SAT_NUMERIC_PASSWORD:
                        case INPUT_TYPE_KEYPAD_NUMERIC:
                            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
                            break;
                        case INPUT_TYPE_DECIMAL_NUMERIC:
                            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY);
                            break;
                        case INPUT_TYPE_NUMERIC:
                        case INPUT_TYPE_NUMERIC_PASSWORD:
                            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
                            break;
                    }
                }
            }
        #endif /* defined(__MMI_TOUCH_SCREEN__) */ 
        }
            break;
        case INLINE_ITEM_TYPE_MULTI_LINE_EDIT:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
        #if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
            MMI_fixed_list_menu.first_displayed_item = MMI_fixed_list_menu.highlighted_item;
        #endif 
        #if defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
            set_left_softkey_label((UI_string_type) get_string(STR_GLOBAL_OPTIONS));
            set_left_softkey_icon(NULL);
        #else /* defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
            set_left_softkey_label(get_string(STR_GLOBAL_OK));
            set_left_softkey_icon(NULL);
        #endif /* defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
            register_left_softkey_handler();
            set_left_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        #if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
            mmi_pen_editor_set_inputbox_LSK_Function(execute_wgui_inline_edit_RSK_function);
        #endif 
            if (wgui_inline_list_menu_changed)
            {
                if (wgui_inline_list_menu_disable_done)
                {
                    inline_text_edit_RSK_label_string = (UI_string_type) get_string(STR_GLOBAL_BACK);
                    inline_text_edit_RSK_label_icon = NULL;
                }
                else
                {
                    inline_text_edit_RSK_label_string = NULL;
                    inline_text_edit_RSK_label_icon = NULL;
                }
            }
            else
            {
                inline_text_edit_RSK_label_string = wgui_inline_edit_get_RSK_string();
                inline_text_edit_RSK_label_icon = NULL;
            }
            set_right_softkey_label(inline_text_edit_RSK_label_string);
            set_right_softkey_icon(inline_text_edit_RSK_label_icon);
        #if defined(__MMI_TOUCH_SCREEN__)
             
            if (!mmi_pen_editor_is_closecategory57)
            {
                 
                multiline_inputbox_category_history *h =
                    (multiline_inputbox_category_history*) wgui_current_history_buffer;
                if (wgui_current_history_buffer && history_valid(h->history_ID, INLINE_FULL_SCREEN_EDIT_HISTORY_ID))
                {
                    wgui_setup_virtual_keyboard(h->vk_lang_type);
                }
                else
                {
                    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) wgui_inline_items[item_index].item;
                    U16 input_type_only = i->input_type & INPUT_TYPE_MASK;

                    switch (input_type_only)
                    {
                        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE:
                        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE:
                        default:
                            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_TRAY);
                            break;
                        case INPUT_TYPE_PHONE_NUMBER:
                            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
                            break;
                        case INPUT_TYPE_SAT_NUMERIC_PASSWORD:
                        case INPUT_TYPE_KEYPAD_NUMERIC:
                            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_PHONE_NUMBER);
                            break;
                        case INPUT_TYPE_DECIMAL_NUMERIC:
                            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_NUMBER_DOT_TRAY);
                            break;
                        case INPUT_TYPE_NUMERIC:
                        case INPUT_TYPE_NUMERIC_PASSWORD:
                            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
                            break;
                    }
                }
            }
        #endif /* defined(__MMI_TOUCH_SCREEN__) */ 
        }
            break;

        case INLINE_ITEM_TYPE_FULL_SCREEN_EDIT:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
            set_left_softkey_label(get_string(WGUI_CATEGORY_EDIT_STRING_ID));
            set_left_softkey_icon(NULL);
            register_left_softkey_handler();
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        #if defined(__MMI_TOUCH_SCREEN__)
             
            if (!(wgui_inline_items[item_index].flags & INLINE_ITEM_OPEN))
            {
                wgui_current_history_buffer = NULL;
            }
        #endif /* defined(__MMI_TOUCH_SCREEN__) */ 
        }
            break;

            // #ifdef __MMI_PLUTO_GPRS__
        case INLINE_ITEM_IMAGE_TEXT_TYPE:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
            set_left_softkey_label(get_string(WGUI_CATEGORY_EDIT_STRING_ID));
            set_left_softkey_icon(NULL);
            register_left_softkey_handler();
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        }
            break;
        case INLINE_ITEM_ATTACH_IMAGE:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
            set_left_softkey_label(get_string(WGUI_CATEGORY_EDIT_STRING_ID));
            set_left_softkey_icon(NULL);
            register_left_softkey_handler();
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);

        }
            break;
 
        case INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        }

            break;
            // #endif
        case INLINE_ITEM_TYPE_SELECT:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
            clear_left_softkey_handler();
            set_left_softkey_label(NULL);
            set_left_softkey_icon(NULL);
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        }
            break;
        case INLINE_ITEM_TYPE_USER_DEFINED_SELECT:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
            clear_left_softkey_handler();
            set_left_softkey_label(NULL);
            set_left_softkey_icon(NULL);
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        }
            break;
        case INLINE_ITEM_TYPE_DATE:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
            set_left_softkey_label(NULL);
            set_left_softkey_icon(NULL);
            register_left_softkey_handler();
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        #if defined(__MMI_TOUCH_SCREEN__)
             
            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
        #endif /* defined(__MMI_TOUCH_SCREEN__) */ 
        }
            break;
        case INLINE_ITEM_TYPE_TIME:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
            set_left_softkey_label(NULL);
            set_left_softkey_icon(NULL);
            register_left_softkey_handler();
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        #if defined(__MMI_TOUCH_SCREEN__)
             
            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
        #endif /* defined(__MMI_TOUCH_SCREEN__) */ 
        }
            break;
        case INLINE_ITEM_TYPE_TIME_PERIOD:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
            set_left_softkey_label(NULL);
            set_left_softkey_icon(NULL);
            register_left_softkey_handler();
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        #if defined(__MMI_TOUCH_SCREEN__)
             
            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
        #endif /* defined(__MMI_TOUCH_SCREEN__) */ 
        }
            break;
        case INLINE_ITEM_TYPE_DOW_SELECT:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }

            if (MMI_inline_DOW_select.states[MMI_inline_DOW_select.highlighted_cell])
            {
                set_left_softkey_label(get_string(WGUI_CATEGORY_OFF_STRING_ID));
            }
            else
            {
                set_left_softkey_label(get_string(WGUI_CATEGORY_ON_STRING_ID));
            }

            set_left_softkey_icon(NULL);
            register_left_softkey_handler();
            set_left_softkey_function(handle_inline_DOW_select_LSK_up, KEY_EVENT_UP);
        #if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
            mmi_pen_editor_set_inputbox_LSK_Function(handle_inline_DOW_select_LSK_up);
        #endif 
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        }
            break;
        case INLINE_ITEM_TYPE_IP4_EDIT:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }
            set_left_softkey_label(NULL);
            set_left_softkey_icon(NULL);
            register_left_softkey_handler();
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        #if defined(__MMI_TOUCH_SCREEN__)
             
            wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_NUMBER_TRAY);
        #endif /* defined(__MMI_TOUCH_SCREEN__) */ 
        }
            break;
        case INLINE_ITEM_TYPE_COLOR_SELECT:
        {
            if (MMI_fixed_list_menu.first_displayed_item == item_index &&
                (wgui_inline_items[item_index - 1].flags == INLINE_ITEM_TYPE_CAPTION))
            {
                MMI_fixed_list_menu.first_displayed_item = item_index - 1;
            }

            clear_left_softkey_handler();
            set_left_softkey_label(NULL);
            set_left_softkey_icon(NULL);
            set_right_softkey_label(wgui_inline_edit_get_RSK_string());
            set_right_softkey_icon(NULL);
            register_right_softkey_handler();
            set_right_softkey_function(execute_wgui_inline_edit_RSK_function, KEY_EVENT_UP);
        }
            break;

    }
#if defined(__MMI_TOUCH_SCREEN__)
     
    if (!mmi_pen_editor_is_closecategory57)
    {
        if ((GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY != MMI_virtual_keyboard.lang_type)
            && (!((current_wgui_inline_item->flags & INLINE_ITEM_TYPE_MASK) == INLINE_ITEM_TYPE_FULL_SCREEN_EDIT)))
        {
            mmi_pen_editor_list_menu_highlight_resize();
        }
        gui_fixed_list_menu_locate_highlighted_item(&MMI_fixed_list_menu);
         
        if (INLINE_ITEM_TYPE_MULTI_LINE_EDIT == (wgui_inline_items[item_index].flags & 0xff))
        {
            MMI_fixed_list_menu.first_displayed_item = item_index;
        }
    }
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
    gdi_layer_lock_frame_buffer();
    MMI_list_highlight_handler(item_index);
    if (!(MMI_fixed_list_menu.n_items == 2 && wgui_inline_items[0].flags == INLINE_ITEM_TYPE_CAPTION))
    {
         
         
        if (GetActiveScreenId() != POPUP_SCREENID
            && GetActiveScreenId() != WAP_SCREEN_MESSAGE_NOTIFICATION
            /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-10 14:5 */
            #ifdef  __MMI_POC__     //dyj add 20060512
            && GetActiveScreenId() != SCR_ID_POC_CM
            #endif
            /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-10 14:5 */
            && GetActiveScreenId() != SCR_ID_SAT_WAITSCREEN
            && GetActiveScreenId() != SCR_POPUP_SCREEN && GetActiveScreenId() != SCR_ID_MSG_POPUP)
        {
        #ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
            show_softkey_background();
        #endif 
            redraw_left_softkey();
            redraw_right_softkey();
        }
    }
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, UI_device_height - MMI_button_bar_height, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  category_inline_edit_menuitem_flags
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void category_inline_edit_menuitem_flags(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_INLINE_ITEMS; i++)
    {
        MMI_fixed_icontext_menuitems[i].flags &= ~UI_MENUITEM_INLINE_EDIT_OBJECT;
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_inline_edit_menuitem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x           [IN]        
 *  y           [IN]        
 *  width       [IN]        
 *  height      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_inline_edit_menuitem(S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((current_fixed_list_menuitem_display_index < 0) ||
        (current_fixed_list_menuitem_display_index > (MMI_fixed_list_menu.n_items - 1)))
    {
        return;
    }
    switch (wgui_inline_items[current_fixed_list_menuitem_display_index].flags & 0xff)
    {
        case INLINE_ITEM_TYPE_DOW_SELECT:
            wgui_show_DOW_list_menuitem(
                x,
                y,
                width,
                height,
                &wgui_inline_items[current_fixed_list_menuitem_display_index]);
            break;
        case INLINE_ITEM_TYPE_TEXT_EDIT:
            wgui_show_inline_text_edit_list_menuitem(
                x,
                y,
                width,
                height,
                &wgui_inline_items[current_fixed_list_menuitem_display_index]);
            break;
        case INLINE_ITEM_TYPE_DATE:
            wgui_show_inline_date_list_menuitem(
                x,
                y,
                width,
                height,
                &wgui_inline_items[current_fixed_list_menuitem_display_index]);
            break;
        case INLINE_ITEM_TYPE_TIME:
            wgui_show_inline_time_list_menuitem(
                x,
                y,
                width,
                height,
                &wgui_inline_items[current_fixed_list_menuitem_display_index]);
            break;
        case INLINE_ITEM_TYPE_TIME_PERIOD:
            wgui_show_inline_time_period_list_menuitem(
                x,
                y,
                width,
                height,
                &wgui_inline_items[current_fixed_list_menuitem_display_index]);
            break;
        case INLINE_ITEM_TYPE_IP4_EDIT:
            wgui_show_inline_IP4_list_menuitem(
                x,
                y,
                width,
                height,
                &wgui_inline_items[current_fixed_list_menuitem_display_index]);
            break;
            // #ifdef __MMI_PLUTO_GPRS__
        case INLINE_ITEM_IMAGE_TEXT_TYPE:
            wgui_show_image_text_item(
                x,
                y,
                width,
                height,
                &wgui_inline_items[current_fixed_list_menuitem_display_index]);
            break;
        case INLINE_ITEM_ATTACH_IMAGE:
            wgui_show_image_attachment_item(
                x,
                y,
                width,
                height,
                &wgui_inline_items[current_fixed_list_menuitem_display_index]);
            break;
            // #endif
        case INLINE_ITEM_TYPE_SELECT:
            wgui_show_inline_select_arrow(
                x,
                y,
                width,
                height,
                &wgui_inline_items[current_fixed_list_menuitem_display_index]);
            break;
    #if defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__)
        case INLINE_ITEM_TYPE_MULTI_LINE_EDIT:
            wgui_show_inline_inscreen_multi_line_edit(
                x,
                y,
                width,
                height,
                &wgui_inline_items[current_fixed_list_menuitem_display_index]);
            break;
        case INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY:
            wgui_show_inline_multi_line_rd_only(
                x,
                y,
                width,
                height,
                &wgui_inline_items[current_fixed_list_menuitem_display_index]);
            break;
    #endif /* defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__) */ 
        case INLINE_ITEM_TYPE_COLOR_SELECT:
            wgui_show_highlighted_inline_color_select_menuitemitem(
                x,
                y,
                width,
                height,
                &wgui_inline_items [current_fixed_list_menuitem_display_index]);
            break;

    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_setup_inline_edit_themes
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_setup_inline_edit_themes(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_set_date_time_input_focussed_theme();
    temp_inline_edit_background_filler = date_time_input_background_filler;
    temp_inline_edit_background_filler3 = date_time_input_background_filler;
    temp_inline_edit_background_filler3.flags &= 0x00000fff;
    wgui_inline_select_fixed_text_menuitem_theme.normal_filler =
        wgui_inline_fixed_icontext_menuitem_theme.normal_filler;
    wgui_inline_select_fixed_text_menuitem_theme.disabled_filler =
        wgui_inline_fixed_icontext_menuitem_theme.disabled_filler;
    wgui_inline_select_fixed_text_menuitem_theme.selected_filler =
        wgui_inline_fixed_icontext_menuitem_theme.selected_filler;
    wgui_inline_select_fixed_text_menuitem_theme.focussed_filler =
        wgui_inline_fixed_icontext_menuitem_theme.focussed_filler;
    wgui_inline_select_fixed_text_menuitem_theme.normal_text_color =
        wgui_inline_fixed_icontext_menuitem_theme.normal_text_color;
    wgui_inline_select_fixed_text_menuitem_theme.disabled_text_color =
        wgui_inline_fixed_icontext_menuitem_theme.disabled_text_color;
    wgui_inline_select_fixed_text_menuitem_theme.selected_text_color =
        wgui_inline_fixed_icontext_menuitem_theme.selected_text_color;
    wgui_inline_select_fixed_text_menuitem_theme.focussed_text_color =
        wgui_inline_fixed_icontext_menuitem_theme.focussed_text_color;
    current_UI_theme->fixed_text_menuitem_theme = &wgui_inline_select_fixed_text_menuitem_theme;
}


/*****************************************************************************
 * FUNCTION
 *  DisableInlineInputMethodHints
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DisableInlineInputMethodHints(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_disable_inline_input_method_hints = 1;
}
/*****************************************************************************
 * FUNCTION
 *  EnableInlineInputMethodHints
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EnableInlineInputMethodHints(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_disable_inline_input_method_hints = 0;
}

/*----------------------------------------------------------------------------
   Category 57 specific functions for inline editing
----------------------------------------------------------------------------*/
#define CAT57_APPEND_NONE                    0x00000000
#define CAT57_APPEND_TOP_INFO_BOX            0x00000001
#define CAT57_APPEND_BUTTOM_INFO_BOX         0x00000002 /* no implement */
#define CAT57_APPEND_TOP_MULTILINE_BOX       0x00000003 /* no implement */
#define CAT57_APPEND_BUTTOM_MULTILINE_BOX    0x00000004 /* no implement */

/* parameter from caller */
static U16 cat57_append_type = CAT57_APPEND_NONE;
static void (*cat57_draw_info_callback) (S32 idx, S32 x1, S32 y1, S32 x2, S32 y2) = NULL;
static void (*cat57_hide_info_callback) (S32 idx) = NULL;
static BOOL cat57_is_draw_once; /* will one callback once */
static U16 cat57_info_row_count;

/* inner parameters */
static S32 cat57_pos_x1;
static S32 cat57_pos_y1;
static S32 cat57_pos_x2;
static S32 cat57_pos_y2;
static BOOL cat57_is_need_draw;


/*****************************************************************************
 * FUNCTION
 *  InitializeCategory57Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void InitializeCategory57Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    n_inline_text_edit_items = 0;
    n_inline_select_items = 0;
    n_inline_date_items = 0;
    n_inline_time_items = 0;
    n_inline_user_defined_select_items = 0;
    n_inline_IP4_items = 0;
    n_inline_DOW_items = 0;
    n_inline_time_period_items = 0;
    wgui_n_inline_items = 0;
    // #ifdef __MMI_PLUTO_GPRS__
    n_inline_image_text = 0;
    n_inline_attach_items = 0;
    n_inline_color_select_items = 0;
    current_attach_control = NULL;
    attach_disable_flag = MMI_FALSE;
    // #endif

    wgui_inline_item_highlighted_index = -1;
    wgui_inline_edit_DONE_function = NULL;
    wgui_inline_edit_BACK_function = NULL;
    wgui_inline_list_menu_changed = 0;
    current_wgui_inline_item = NULL;
    wgui_inline_edit_arrow_key_flag = CATEGORY57_NO_KEY_FLAG;
     
    flag_scroll = FALSE;
     
     
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    /* To enable line display */
    gui_enable_inline_bottom_line_draw();
    gui_enable_fixed_list_line_draw();
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
     
    wgui_disable_inline_input_method_hints = 1;

     
    memset(wgui_inline_items, 0, sizeof(wgui_inline_item) * MAX_INLINE_ITEMS);
     

     
    inline_item_changed_handler = NULL;
     

}


/*****************************************************************************
 * FUNCTION
 *  SetCategory57RightSoftkeyFunctions
 * DESCRIPTION
 *  
 * PARAMETERS
 *  done_function       [IN]        
 *  back_function       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory57RightSoftkeyFunctions(void (*done_function) (void), void (*back_function) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_edit_DONE_function = done_function;
    wgui_inline_edit_BACK_function = back_function;
    inline_text_edit_set_RSK_function();
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory57LeftSoftkeyFunction
 * DESCRIPTION
 *  
 * PARAMETERS
 *  LSK_function        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory57LeftSoftkeyFunction(void (*LSK_function) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_left_softkey_handler();
    set_left_softkey_function(execute_wgui_inline_edit_LSK_function, KEY_EVENT_UP);
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_set_inputbox_LSK_Function(execute_wgui_inline_edit_LSK_function);
#endif 
    wgui_inline_edit_LSK_function = LSK_function;
}


/*****************************************************************************
 * FUNCTION
 *  DisableCategory57ScreenDone
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DisableCategory57ScreenDone(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_list_menu_disable_done = 1;
}


/*****************************************************************************
 * FUNCTION
 *  EnableCategory57ScreenDone
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EnableCategory57ScreenDone(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_list_menu_disable_done = 0;
}

 


/*****************************************************************************
 * FUNCTION
 *  SetCategory57ScreenRSKClear
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory57ScreenRSKClear(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_list_menu_disable_done = 2; /* clear */
}


/*****************************************************************************
 * FUNCTION
 *  cat257_virtual_keypad_callback
 * DESCRIPTION
 *  Redraws the category57 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 
UI_font_type cat57_f1;
UI_font_type cat57_f2;

//----------------------------------------------------------------------------
 
U16 cat57_img;

void cat257_virtual_keypad_callback(void)
{
#if defined(__MMI_TOUCH_SCREEN__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY != MMI_virtual_keyboard.lang_type)
    {
        mmi_pen_editor_clear_and_show_virtual_keyboard_area();
    }
    else
    {
    }
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
}

 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory257Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  ImgId                   [IN]        
 *  number_of_items         [IN]        
 *  list_of_icons           [?]         
 *  list_of_items           [?]         
 *  highlighted_item        [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory257Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 ImgId,
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

#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    color_t c = {0, 0, 255, 100};  
#else 
    color_t c = {255, 255, 255, 100};
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    /* to avoid grid display on this cateory screen */
    gui_disable_fixed_list_line_draw();
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 

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
        wgui_inline_edit_default_list_highlight_handler,
        wgui_inline_edit_default_list_unhighlight_handler,
        MMI_CATEGORY57_ID);

    gdi_layer_unlock_frame_buffer();

    if (list_of_icons == NULL)
    {
        set_fixed_icontext_positions(4, 0, 1, 0);
    }
    else
    {
        set_fixed_icontext_positions(MMI_INLINE_EDIT_MENUITEM_HEIGHT + 2, 0, 1, 0);
    }
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_SCROLLBAR | UI_LIST_MENU_DISABLE_BACKGROUND;

#ifdef __MMI_UI_LIST_CACHE_BACKGROUND__
     
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_BKGRND_IN_LAYER;
#endif /* __MMI_UI_LIST_CACHE_BACKGROUND__ */ 
    SetCategory57ScreenRSKClear();
    SetCategory57RightSoftkeyFunctions(NULL, GoBackHistory);
    ExitCategoryFunction = ExitCategory57Screen;
    GetCategoryHistory = GetCategory57History;
    GetCategoryHistorySize = GetCategory57HistorySize;
    RedrawCategoryFunction = dm_redraw_category_screen;
    dm_add_image(get_image(ImgId), NULL, NULL);
#ifdef __MMI_MAINLCD_220X176__
/*The rectangle doesn't need to be filled from inside.*/
    dm_add_rectangle(c, 0);
#else
    dm_add_rectangle(c, 1);
#endif
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY257_ID;
#ifdef __MMI_MAINLCD_220X176__
/*Clear the background as black.*/
    dm_data.S32flags = DM_SHOW_VKPAD|DM_CLEAR_SCREEN_BACKGROUND;
#else
    dm_data.S32flags = DM_SHOW_VKPAD;
#endif
    dm_setup_data(&dm_data);
    dm_register_vkpad_callback(cat57_virtual_keypad_callback);
    dm_redraw_category_screen();
}

/*****************************************************************************
*
* Category 57
*
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  Cat57DrawTopInfoBox
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void Cat57DrawTopInfoBox(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gui_set_clip(cat57_pos_x1, cat57_pos_y1, cat57_pos_x2, cat57_pos_y2);

    gui_fill_rectangle(
        cat57_pos_x1,
        cat57_pos_y1,
        cat57_pos_x2,
        cat57_pos_y2,
        current_MMI_theme->list_selected_item_filler->c);

    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory57Screen
 * DESCRIPTION
 *  Category57 redraw function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory57Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  dm_category57_controlled_area
 * DESCRIPTION
 *  Category 57 controlled area callback called by draw manager
 * PARAMETERS
 *  coordinate      [IN/OUT]        It specifies the area in which the category will start the drawing. Also modifies it.
 * RETURNS
 *  void
 *****************************************************************************/
void dm_category57_controlled_area(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (cat57_append_type == CAT57_APPEND_TOP_INFO_BOX)
    {
        cat57_is_need_draw = TRUE;
        coordinate->S16Height = MMI_menuitem_height * cat57_info_row_count;
        /* draw top info box */
        Cat57DrawTopInfoBox();
        if (cat57_draw_info_callback != NULL)
        {
            cat57_draw_info_callback(
                MMI_fixed_list_menu.highlighted_item,
                cat57_pos_x1,
                cat57_pos_y1,
                cat57_pos_x2,
                cat57_pos_y2);
        }
    }
    else
    {
        coordinate->S16Width = 0;
        coordinate->S16Height = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  Cat57HighlightHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item_index      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void Cat57HighlightHandler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    wgui_inline_edit_default_list_highlight_handler(item_index);

    if (cat57_append_type == CAT57_APPEND_TOP_INFO_BOX)
    {
        if (!(wgui_inline_items[item_index].flags & INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT))
        {
            if (cat57_draw_info_callback != NULL && cat57_is_draw_once == FALSE)
            {
                Cat57DrawTopInfoBox();
                cat57_draw_info_callback(item_index, cat57_pos_x1, cat57_pos_y1, cat57_pos_x2, cat57_pos_y2);
            }
        }
    }

    gdi_layer_unlock_frame_buffer();
  //  gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  Cat57UnHighlightHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item_index      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void Cat57UnHighlightHandler(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_edit_default_list_unhighlight_handler(item_index);

    if (cat57_hide_info_callback != NULL && cat57_is_draw_once == FALSE)
    {
        cat57_hide_info_callback(item_index);
    }

}


/*****************************************************************************
 * FUNCTION
 *  cat57_virtual_keypad_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat57_virtual_keypad_callback(void)
{
#if defined(__MMI_TOUCH_SCREEN__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    //if (MMI_FALSE == dm_is_redraw_screen())
    //{
    //   mmi_pen_editor_store_list_menu_height(MMI_fixed_list_menu.height - 1);
    //}
     
    if ((GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY != MMI_virtual_keyboard.lang_type)
        && (!((current_wgui_inline_item->flags & INLINE_ITEM_TYPE_MASK) == INLINE_ITEM_TYPE_FULL_SCREEN_EDIT)))
    {
         
        //resize_fixed_list(MMI_content_width,mmi_pen_editor_get_list_menu_height() - MMI_virtual_keyboard.height - 1);
        //mmi_pen_editor_vk_list_menu_resize();
        if ((current_wgui_inline_item->flags & INLINE_ITEM_TYPE_MASK) == INLINE_ITEM_TYPE_MULTI_LINE_EDIT)
        {
            mmi_pen_editor_resize_multiline_input_box_for_vk();
        }
        else
        {
            mmi_pen_editor_clear_and_show_virtual_keyboard_area();
        }
    }
    //else
    //{
    //   resize_fixed_list(MMI_content_width,mmi_pen_editor_get_list_menu_height());
    //   redraw_fixed_list();
    //}
    //redraw_current_inline_item();
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 
}

 
#if defined (__MMI_WALLPAPER_ON_BOTTOM__)
static U8 cat357_scr_bg_opacity;
static BOOL cat357_opacity_changed = FALSE;


/*****************************************************************************
 * FUNCTION
 *  ShowCategory357Screen
 * DESCRIPTION
 *  Redraws the category357 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  ImgId                   [IN]        Image shown on top of list
 *  number_of_items         [IN]        Number of items
 *  list_of_icons           [IN]        List of icons
 *  list_of_items           [IN]        See InlineItem structure.
 *  highlighted_item        [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory357Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 ImgId,
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

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    //U16 list_offset_y;

#if defined(__MMI_UI_DALMATIAN_FIXEDLIST__)
    gui_disable_fixed_list_line_draw();
#endif 

    gdi_layer_lock_frame_buffer();

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
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
        MMI_CATEGORY57_ID);

    if (flag_scroll)
    {
        MMI_fixed_list_menu.flags |= UI_LIST_MENU_DISABLE_SCROLLBAR;
    }

    dm_add_image(get_image(ImgId), NULL, NULL);
    dm_get_scr_bg_opacity(&cat357_scr_bg_opacity);
    dm_set_scr_bg_opacity(0);
    cat357_opacity_changed = TRUE;

    gdi_layer_unlock_frame_buffer();

    ExitCategoryFunction = ExitCategory57Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = GetCategory57History;
    GetCategoryHistorySize = GetCategory57HistorySize;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY357_ID;
    dm_data.S32flags = 0;
    dm_data.S32flags |= DM_SHOW_VKPAD;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(dm_category57_controlled_area);
    dm_register_vkpad_callback(cat57_virtual_keypad_callback);
    dm_redraw_category_screen();
}
#endif /* defined (__MMI_WALLPAPER_ON_BOTTOM__) */ 
 

#ifdef _KK_SUPPORT_
//#include "DevExtWguiinlineedit.h"

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
#if 0
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
#endif
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
#if 1
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
#endif
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
    dm_data.S32ScrId = (S32) GetActiveScreenId();
   dm_data.S32CatId = MMI_CATEGORY57_WEP_VIDEO_CHAT_ID;
    dm_data.S32flags = 0;
    dm_data.S32flags |= DM_SHOW_VKPAD;
    dm_data.S32flags |= DM_NO_BLT ;	
    dm_data.S32flags |=DM_NO_TITLE;
//    dm_data.S32flags |=DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(dm_category57_controlled_area);
    dm_register_vkpad_callback(cat57_virtual_keypad_callback);
    dm_redraw_category_screen();
}

#endif

#ifdef __MMI_MAINLCD_220X176__
#include "settingdefs.h"
extern void GbInuptMethodEnterSinglelineInputBox();
#endif
/*****************************************************************************
 * FUNCTION
 *  ShowCategory57Screen
 * DESCRIPTION
 *  Redraws the category57 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  number_of_items         [IN]        Number of items
 *  list_of_icons           [IN]        List of icons
 *  list_of_items           [IN]        See InlineItem structure.
 *  highlighted_item        [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory57Screen(
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
    /* init append related parameters */
    if (cat57_append_type == CAT57_APPEND_TOP_INFO_BOX)
    {
        list_height = MMI_content_height - MMI_menuitem_height * cat57_info_row_count;
        list_offset_y = MMI_title_y + MMI_title_height + MMI_menuitem_height * cat57_info_row_count;

        cat57_pos_x1 = 0;
        cat57_pos_y1 = MMI_title_y + MMI_title_height;
        cat57_pos_x2 = UI_device_width - 1;
        cat57_pos_y2 = MMI_title_y + MMI_title_height + MMI_menuitem_height * cat57_info_row_count - 1;

        cat57_is_need_draw = FALSE;
    }

#if defined(__MMI_UI_DALMATIAN_FIXEDLIST__)
    gui_disable_fixed_list_line_draw();
#endif 

    gdi_layer_lock_frame_buffer();

#if defined(__MMI_TOUCH_SCREEN__)
    wgui_setup_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY);
#endif 

#ifdef __MMI_MAINLCD_220X176__
	if (SCR_ID_PHNSET_SET_DT == GetActiveScreenId())
	   GbInuptMethodEnterSinglelineInputBox();
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
        MMI_CATEGORY57_ID);

    if (flag_scroll)
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
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY57_ID;
    dm_data.S32flags = 0;
    dm_data.S32flags |= DM_SHOW_VKPAD;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(dm_category57_controlled_area);
    dm_register_vkpad_callback(cat57_virtual_keypad_callback);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  Category57AppendTopInfoBox
 * DESCRIPTION
 *  Append top info box to Category screen 57.
 *  [NOTIC!] call this function before calling ShowCategory57Screen
 * PARAMETERS
 *  num_of_info_box_rows        [IN]        
 *  draw_info_callback          [IN]        
 *  hide_info_callback          [IN]        
 *  is_draw_once                [IN]        
 *  U16(?)                      [IN]        Row count for info box
 * RETURNS
 *  void
 *****************************************************************************/
void Category57AppendTopInfoBox(
        U16 num_of_info_box_rows,
        void (*draw_info_callback) (S32 idx, S32 x1, S32 y1, S32 x2, S32 y2),
        void (*hide_info_callback) (S32 idx),
        BOOL is_draw_once)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cat57_append_type = CAT57_APPEND_TOP_INFO_BOX;

    cat57_draw_info_callback = draw_info_callback;
    cat57_hide_info_callback = hide_info_callback;
    cat57_info_row_count = num_of_info_box_rows;
    cat57_is_draw_once = is_draw_once;
}


/*****************************************************************************
 * FUNCTION
 *  CloseCategory57Screen
 * DESCRIPTION
 *  Exits the category57 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void reset_pop_up_descriptions(void);

void CloseCategory57Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
#if defined(__MMI_TOUCH_SCREEN__)
    mmi_pen_editor_is_closecategory57 = MMI_TRUE;
#endif 
    complete_inline_item_edit();
#if defined(__MMI_TOUCH_SCREEN__)
    mmi_pen_editor_is_closecategory57 = MMI_FALSE;
#endif 
    gdi_layer_unlock_frame_buffer();
}
extern void UI_dummy_menuitem_displayed_function(S32 index, S32 x1, S32 y1, S32 x2, S32 y2);


/*****************************************************************************
 * FUNCTION
 *  ExitCategory57Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory57Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__)
    MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_INSIDE_INLINE_MENU;
#endif 

#if(UI_BLINKING_CURSOR_SUPPORT)
    //gui_cancel_timer(UI_inputbox_blink_cursor);
     
    StopMyTimer(BLINKING_CURSOR);
    /* CSD end */
#endif /* (UI_BLINKING_CURSOR_SUPPORT) */ 

     
#if defined(__MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__) && defined(__MMI_UI_DALMATIAN_FIXEDLIST__)
    if (IsInlineItemDefaultTextEnable())
    {
        gui_enable_fixed_list_line_draw();
    }
#endif /* defined(__MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__) && defined(__MMI_UI_DALMATIAN_FIXEDLIST__) */ 

#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    DisableInlineItemDefaultText();
#endif 
     
    MMI_disable_title_shortcut_display = 0;
    wgui_inline_edit_arrow_key_flag = CATEGORY57_NO_KEY_FLAG;
    gui_set_fixed_list_menu_current_theme(&MMI_fixed_list_menu);
    current_wgui_inline_item = NULL;
    wgui_n_inline_items = 0;
     
#ifdef __MMI_UI_DALMATIAN_FIXEDLIST__
    /* reset the original settings for grid display */
    gui_disable_inline_bottom_line_draw();
    gui_enable_fixed_list_line_draw();
#endif /* __MMI_UI_DALMATIAN_FIXEDLIST__ */ 
     
     
    flag_use_current_input_type = FALSE;
     
    wgui_inline_item_highlighted_index = -1;
    wgui_inline_list_menu_changed = 0;
    wgui_inline_list_menu_disable_done = 0;
    MMI_multi_line_inputbox_present = 0;
     
    wgui_inputbox_information_flag = 0;
     
    wgui_inline_edit_DONE_function = NULL;
    wgui_inline_edit_BACK_function = NULL;
    complete_inline_item_edit = UI_dummy_function;
    category_inline_edit_menuitem_flags();
    clear_inline_item_keys();
    wgui_text_menuitem_reset_scrolling();
    /* ifdef __MMI_PLUTO_GPRS__ */
    clear_inline_image_text_item();
    clear_inline_image_attachment_item();
    //endif
     
    MMI_multiline_inputbox.editor_changed = 0;
    reset_pop_up_descriptions();
    redraw_current_inline_item = UI_dummy_function;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = UI_dummy_inline_item_pen_function;
#endif 
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    MMI_fixed_list_menu.item_displayed_callback = UI_dummy_menuitem_displayed_function;
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    inline_edit_current_highlight_handler = MMI_dummy_highlight_handler;
    MMI_fixed_icontext_menuitem.text_font = cat57_f1;
    MMI_fixed_text_menuitem.text_font = cat57_f2;

    /* reset append */
    cat57_append_type = CAT57_APPEND_NONE;
    cat57_draw_info_callback = NULL;
    cat57_hide_info_callback = NULL;
    cat57_info_row_count = 0;

#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    if (cat357_opacity_changed)
    {
        dm_set_scr_bg_opacity(cat357_scr_bg_opacity);
    }
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 

     
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_unblock_list_effect();
#endif 
     

     
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    enable_fixed_icontext_menuitem_transparent_effect();
#endif 
     
     
    inline_item_changed_handler = NULL;
     
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory57HistorySize
 * DESCRIPTION
 *  Gets the size of the history buffer for the category57 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  size of history buffer in U8s
 *****************************************************************************/
S32 GetCategory57HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (wgui_get_inline_history_size());
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory57History
 * DESCRIPTION
 *  Gets the history buffer for the category57 screen
 * PARAMETERS
 *  history_buffer      [IN]        Is the buffer into which the history data is stored (pre-allocated)
 * RETURNS
 *  pointer to the history buffer
 *****************************************************************************/
U8 *GetCategory57History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (wgui_get_inline_history(MMI_CATEGORY57_ID, history_buffer));
}


/*****************************************************************************
 * FUNCTION
 *  Category57ChangeItemText
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 *  text        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void Category57ChangeItemText(S32 index, U8 *text)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_icontext_menuitems[index].item_text = (UI_string_type) text;
}


/*****************************************************************************
 * FUNCTION
 *  Category57ChangeItemIcon
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index           [IN]        
 *  image_ID        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category57ChangeItemIcon(S32 index, U16 image_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* FIXME. There is potential problem that some inline items does not have icon. */
    MMI_fixed_icontext_menuitems[index].item_icon = wgui_get_list_menu_icon(index, image_ID);
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory57DataSize
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 GetCategory57DataSize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (wgui_get_inline_data_size());
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory57Data
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 *GetCategory57Data(U8 *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (wgui_get_inline_data(data));
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory57Data
 * DESCRIPTION
 *  
 * PARAMETERS
 *  list_of_items       [?]         
 *  number_of_items     [IN]        
 *  data                [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory57Data(InlineItem *list_of_items, S32 number_of_items, U8 *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_set_inline_data(list_of_items, number_of_items, data);
}

/*----------------------------------------------------------------------------
   Category 157 specific functions for inline editing
----------------------------------------------------------------------------*/


/*****************************************************************************
 * FUNCTION
 *  InitializeCategory157Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void InitializeCategory157Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    n_inline_text_edit_items = 0;
    n_inline_select_items = 0;
    n_inline_date_items = 0;
    n_inline_time_items = 0;
    n_inline_user_defined_select_items = 0;
    n_inline_IP4_items = 0;
    n_inline_DOW_items = 0;
    n_inline_time_period_items = 0;
    wgui_n_inline_items = 0;

    wgui_inline_item_highlighted_index = -1;
    wgui_inline_edit_DONE_function = NULL;
    wgui_inline_edit_BACK_function = NULL;
    wgui_inline_list_menu_changed = 0;
    current_wgui_inline_item = NULL;
    wgui_inline_edit_arrow_key_flag = CATEGORY57_NO_KEY_FLAG;

    wgui_disable_inline_input_method_hints = 1;
     
    inline_item_changed_handler = NULL;
     
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory157RightSoftkeyFunctions
 * DESCRIPTION
 *  
 * PARAMETERS
 *  done_function       [IN]        
 *  back_function       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory157RightSoftkeyFunctions(void (*done_function) (void), void (*back_function) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_edit_DONE_function = done_function;
    wgui_inline_edit_BACK_function = back_function;
    inline_text_edit_set_RSK_function();
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory157LeftSoftkeyFunction
 * DESCRIPTION
 *  
 * PARAMETERS
 *  LSK_function        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory157LeftSoftkeyFunction(void (*LSK_function) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_left_softkey_handler();
    set_left_softkey_function(execute_wgui_inline_edit_LSK_function, KEY_EVENT_UP);
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
    mmi_pen_editor_set_inputbox_LSK_Function(execute_wgui_inline_edit_LSK_function);
#endif 
    wgui_inline_edit_LSK_function = LSK_function;
}


/*****************************************************************************
 * FUNCTION
 *  DisableCategory157ScreenDone
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void DisableCategory157ScreenDone(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_inline_list_menu_disable_done = 1;
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory157Screen
 * DESCRIPTION
 *  Redraws the category157 screen
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  number_of_items         [IN]        Number of items
 *  list_of_icons           [IN]        List of icons
 *  list_of_items           [IN]        See InlineItem structure.
 *  highlighted_item        [IN]        Default item to be highlighted (if there is no history)
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory157Screen(
        U8 *title,
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

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowCommonCategoryInlineEdit(
        (UI_string_type) title,
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
        wgui_inline_edit_default_list_highlight_handler,
        wgui_inline_edit_default_list_unhighlight_handler,
        MMI_CATEGORY57_ID);

    ExitCategoryFunction = ExitCategory157Screen;
    GetCategoryHistory = GetCategory57History;
    GetCategoryHistorySize = GetCategory57HistorySize;
    RedrawCategoryFunction = dm_redraw_category_screen;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY157_ID;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  CloseCategory157Screen
 * DESCRIPTION
 *  Exits the category157 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void reset_pop_up_descriptions(void);

void CloseCategory157Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    complete_inline_item_edit();
    gdi_layer_unlock_frame_buffer();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory157Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory157Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if(UI_BLINKING_CURSOR_SUPPORT)
    StopMyTimer(BLINKING_CURSOR);
#endif 
    wgui_inline_edit_arrow_key_flag = CATEGORY57_NO_KEY_FLAG;
    gui_set_fixed_list_menu_current_theme(&MMI_fixed_list_menu);
    current_wgui_inline_item = NULL;
    wgui_n_inline_items = 0;
    wgui_inline_item_highlighted_index = -1;
    wgui_inline_list_menu_changed = 0;
    wgui_inline_list_menu_disable_done = 0;
    wgui_inline_edit_DONE_function = NULL;
    wgui_inline_edit_BACK_function = NULL;
    complete_inline_item_edit = UI_dummy_function;
    category_inline_edit_menuitem_flags();
    clear_inline_item_keys();
    wgui_text_menuitem_reset_scrolling();
    reset_pop_up_descriptions();
    redraw_current_inline_item = UI_dummy_function;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_current_inline_item_pen_function = UI_dummy_inline_item_pen_function;
#endif 
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    inline_edit_current_highlight_handler = MMI_dummy_highlight_handler;
    MMI_fixed_icontext_menuitem.text_font = cat57_f1;
    MMI_fixed_text_menuitem.text_font = cat57_f2;
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_unblock_list_effect();
#endif 
#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    enable_fixed_icontext_menuitem_transparent_effect();
#endif 
    inline_item_changed_handler = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  Category157ChangeItemText
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 *  text        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void Category157ChangeItemText(S32 index, U8 *text)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_icontext_menuitems[index].item_text = (UI_string_type) text;
}


/*****************************************************************************
 * FUNCTION
 *  Category157ChangeItemIcon
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index           [IN]        
 *  image_ID        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category157ChangeItemIcon(S32 index, U16 image_ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_icontext_menuitems[index].item_icon = (PU8) get_image(image_ID);
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory157DataSize
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 GetCategory157DataSize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (wgui_get_inline_data_size());
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory157Data
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 *GetCategory157Data(U8 *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (wgui_get_inline_data(data));
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory157Data
 * DESCRIPTION
 *  
 * PARAMETERS
 *  list_of_items       [?]         
 *  number_of_items     [IN]        
 *  data                [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory157Data(InlineItem *list_of_items, S32 number_of_items, U8 *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_set_inline_data(list_of_items, number_of_items, data);
}

#if defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__)
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
U8 inline_multi_line_edit_changed = 0;
UI_string_type inscreen_text_box_title;

U8 inscreen_multi_line_change_flag = 0;
extern void GoBackHistory(void);


/*****************************************************************************
 * FUNCTION
 *  SetInlineItemMultiLineEdit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item            [?]         
 *  buffer          [?]         
 *  title           [?]         
 *  buffer_size     [IN]        
 *  input_type      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineItemMultiLineEdit(InlineItem *item, U8 *buffer, U8 *title, S32 buffer_size, S16 input_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i;
    UI_buffer_type mask_buffer;
    S16 t = input_type & INPUT_TYPE_MASK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inscreen_text_box_title = (UI_string_type) title;
    if (n_inline_text_edit_items >= MAX_INLINE_TEXT_EDIT_ITEMS)
    {
        return;
    }
    item->item = (void*)&wgui_inline_text_edit_items[n_inline_text_edit_items];
    item->flags = INLINE_ITEM_TYPE_MULTI_LINE_EDIT;
    mask_buffer = inline_text_edit_mask_buffers[n_inline_text_edit_items];
    n_inline_text_edit_items++;
    i = (wgui_inline_item_text_edit*) item->item;
    i->max_lines = 0;
    i->title = 0;
    i->title_icon = 0;
    i->buffer = buffer;
    i->buffer_size = buffer_size;
    i->flags = 0;
    i->input_type = input_type;
    item->text_p = i->buffer;
    item->handler = handle_inline_multi_line_edit;
    i->edit_complete_callback = UI_dummy_edit_complete_handler;
    i->custom_setup_function = NULL;
    i->validation_callback = UI_dummy_validation_function;

    if ((t == INPUT_TYPE_NUMERIC_PASSWORD) || (t == INPUT_TYPE_ALPHANUMERIC_PASSWORD) ||
        (t == INPUT_TYPE_SAT_NUMERIC_PASSWORD))
    {
        inline_edit_prepare_mask_buffer(buffer, mask_buffer);
        i->mask_buffer = mask_buffer;
        item->text_p = mask_buffer;
    }
    i->UCS2_count = -1;
    MMI_multi_line_inputbox_present = 1;
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineMultiLineEditCustomFunction
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item        [?]         
 *  f           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineMultiLineEditCustomFunction(InlineItem *item, void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) item->item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i->custom_setup_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_multi_line_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_multi_line_edit(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 input_type;
    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) item;
    S32 inputbox_x = x;
    S32 inputbox_y = y;
    S32 inputbox_width = width + 2;

#if !defined(__MMI_TOUCH_SCREEN__)
    S32 inputbox_height = MMI_content_height - (MMI_fixed_list_menu.n_items) * MMI_ICONTEXT_MENUITEM_HEIGHT - INFORMATION_BAR_HEIGHT - 2;       /* Height Changed According to Menu Item Number. */
#else 
    S32 inputbox_height = MMI_content_height - MMI_MULTITAP_HEIGHT - INFORMATION_BAR_HEIGHT - 7;        /* Height Changed According to Menu Item Number. */
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_multi_line_inputbox_present = 1;
    wgui_current_inline_item_type = INLINE_ITEM_TYPE_MULTI_LINE_EDIT;   /* Should Be Present. */

    UI_UNUSED_PARAMETER(key_code);
    UI_UNUSED_PARAMETER(key_event);
    UI_UNUSED_PARAMETER(flags);

    gdi_layer_lock_frame_buffer();
    wgui_text_menuitem_reset_scrolling();
    if (i->flags & INLINE_ITEM_CHANGED)
    {
        inline_multi_line_edit_changed = 1;
    }
    else
    {
        inline_multi_line_edit_changed = 0;
    }
    wgui_current_inline_item_type = INLINE_ITEM_TYPE_MULTI_LINE_EDIT;
    current_wgui_inline_text_edit_item = i;
    current_wgui_inline_text_edit_text_p = text_p;
    input_type = i->input_type;
    if (!(flags & INLINE_ITEM_OPEN))
    {
        gui_strcpy((UI_string_type) wgui_inline_full_screen_text_edit_buffer, (UI_string_type) i->buffer);
        history_buffer = NULL;
    }
    i->flags &= ~INLINE_ITEM_OPEN;

    register_hide_multitap(category5_hide_multitap);

    wgui_inputbox_validation_callback = i->validation_callback;
    wgui_multiline_setup_inputbox(
        inputbox_x,
        inputbox_y,
        inputbox_width,
        inputbox_height,
        wgui_inline_full_screen_text_edit_buffer,
        i->buffer_size,
        INLINE_MULTI_LINE_EDIT_HISTORY_ID,
        inline_text_edit_RSK_label_string,
        inline_text_edit_RSK_label_icon,
        (S16) input_type,
        history_buffer,
        1);

#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
    dm_get_scr_bg_image(
        &inline_edit_scr_bg_ID,
        &inline_edit_scr_bg_filename,
        &inline_edit_scr_bg_x,
        &inline_edit_scr_bg_y,
        &inline_edit_scr_bg_opacity);
    dm_set_scr_bg_image(inline_edit_scr_bg_ID, inline_edit_scr_bg_filename, inline_edit_scr_bg_x, inline_edit_scr_bg_y, MMI_EDITOR_BG_OPACITY);  
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 

#if defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
    set_left_softkey_label((UI_string_type) get_string(STR_GLOBAL_OPTIONS));
    set_left_softkey_icon(NULL);
    SetLeftSoftkeyFunction(EntryInLineMultilineEditorOption, KEY_EVENT_UP);
#endif /* defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    redraw_right_softkey();
    redraw_left_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_multiline_inputbox_input_callback(wgui_inline_edit_handle_multiline_inputbox_input);
    wgui_inputbox_RSK_function = wgui_inline_edit_BACK_function;
    redraw_current_inline_item = show_multiline_inputbox;
    if (i->custom_setup_function != NULL)
    {
        i->custom_setup_function();
    }
    gdi_layer_unlock_frame_buffer();
    show_multiline_inputbox();
    complete_inline_item_edit = handle_inline_multi_line_edit_complete;
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_multi_line_edit_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_multi_line_edit_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 input_type;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    handle_inline_full_screen_edit_close();
    input_type = current_wgui_inline_text_edit_item->input_type & INPUT_TYPE_MASK;
    current_wgui_inline_item->flags &= ~INLINE_ITEM_OPEN;
    if (inline_full_screen_edit_changed)
    {
        wgui_inline_list_menu_changed = 1;
    }
    current_wgui_inline_text_edit_item->UCS2_count = MMI_multiline_inputbox.UCS2_count;
    gui_strcpy(
        (UI_string_type) current_wgui_inline_text_edit_item->buffer,
        (UI_string_type) wgui_inline_full_screen_text_edit_buffer);
    if ((input_type == INPUT_TYPE_NUMERIC_PASSWORD) || (input_type == INPUT_TYPE_ALPHANUMERIC_PASSWORD) ||
        (input_type == INPUT_TYPE_SAT_NUMERIC_PASSWORD))
    {
        inline_edit_prepare_mask_buffer(
            current_wgui_inline_text_edit_item->buffer,
            current_wgui_inline_text_edit_item->mask_buffer);
        *current_wgui_inline_text_edit_text_p = current_wgui_inline_text_edit_item->mask_buffer;
    }
    else
    {
        *current_wgui_inline_text_edit_text_p = current_wgui_inline_text_edit_item->buffer;
    }

    reset_softkeys();
    reset_multitaps();
     
#if !defined(__MMI_TOUCH_SCREEN__)
    wgui_close_inputbox();
#endif 
    clear_left_softkey();
    clear_right_softkey();

    register_left_softkey_handler();
    register_right_softkey_handler();

    wgui_inline_item_handled = 0;

    current_wgui_inline_text_edit_item->edit_complete_callback(
                                            current_wgui_inline_text_edit_item->buffer,
                                            current_wgui_inline_text_edit_item->buffer_size);

    inline_edit_current_highlight_handler(wgui_inline_item_highlighted_index);
    redraw_current_inline_item = UI_dummy_function;
    wgui_inputbox_UCS2_count_function = NULL;
    wgui_inputbox_not_empty_callback = NULL;
    wgui_inputbox_empty_callback = NULL;
    wgui_inputbox_validation_callback = NULL;
    inline_full_screen_edit_changed = 0;
    complete_inline_item_edit = UI_dummy_function;
    current_wgui_inline_text_edit_item->input_type = MMI_current_input_type;
    StopMyTimer(BLINKING_CURSOR);
    gdi_layer_unlock_frame_buffer();
    gdi_layer_blt_previous(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_inline_inscreen_multi_line_edit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  width       [IN]        
 *  height      [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_inline_inscreen_multi_line_edit(S32 x1, S32 y1, S32 width, S32 height, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 input_type;

    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) item->item;
    S32 inputbox_x = x1;
    S32 inputbox_y = y1;
    S32 inputbox_width = width;

#if(INSCREEN_MUTLILINE_TITLE_ENABLE)
#if !defined(__MMI_TOUCH_SCREEN__)
    S32 inputbox_height =
        MMI_content_height - (MMI_fixed_list_menu.n_items) * MMI_ICONTEXT_MENUITEM_HEIGHT - 2 - INFORMATION_BAR_HEIGHT;
#else /* !defined(__MMI_TOUCH_SCREEN__) */ 
    S32 inputbox_height = MMI_content_height - MMI_MULTITAP_HEIGHT - 7 - INFORMATION_BAR_HEIGHT;
#endif /* !defined(__MMI_TOUCH_SCREEN__) */ 
#else /* (INSCREEN_MUTLILINE_TITLE_ENABLE) */ 
#if !defined(__MMI_TOUCH_SCREEN__)
    S32 inputbox_height = MMI_content_height - (MMI_fixed_list_menu.n_items) * MMI_ICONTEXT_MENUITEM_HEIGHT - 2;
#else 
    S32 inputbox_height = MMI_content_height - MMI_MULTITAP_HEIGHT - 7;
#endif 
#endif /* (INSCREEN_MUTLILINE_TITLE_ENABLE) */ 

    S32 fh;
    S32 l;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    input_type = i->input_type;
    wgui_set_default_input_type(&input_type);
    if (flag_use_current_input_type)
    {
        input_type = MMI_current_input_type;
    }
    switch (input_type & INPUT_TYPE_MASK)
    {
        case INPUT_TYPE_NUMERIC_CHANGEABLE:
            MMI_current_input_mode = 2;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_NUMERIC:
            MMI_current_input_mode = 2;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_DECIMAL_NUMERIC:
            MMI_current_input_mode = 2;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_KEYPAD_NUMERIC:
            MMI_current_input_mode = 2;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE:
            MMI_current_input_mode = 0;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE:
            MMI_current_input_mode = 1;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

    #ifdef __MMI_WCSS_INPUT_FORMAT_SUPPORT__
        case INPUT_TYPE_ALPHABATIC_UPPERCASE:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_ABC_NO_NUMERIC;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHABATIC_LOWERCASE:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_ABC_NO_NUMERIC;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_NUMERIC_SYMBOL:
            MMI_current_input_mode = INPUT_MODE_123_SYMBOLS;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* __MMI_WCSS_INPUT_FORMAT_SUPPORT__ */ 

        case INPUT_TYPE_ALPHANUMERIC_SENTENCECASE:
            MMI_current_input_mode = 1;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_NUMERIC_PASSWORD:
            MMI_current_input_mode = 2;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_SAT_NUMERIC_PASSWORD:
            MMI_current_input_mode = 2;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_PASSWORD:
            MMI_current_input_mode = 1;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_PHONE_NUMBER:
            MMI_current_input_mode = 2;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

    #if defined (__MMI_MULTITAP_THAI__)
        case INPUT_TYPE_ALPHANUMERIC_MULTITAP_THAI:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_THAI;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined (__MMI_MULTITAP_THAI__) */ 

    #if defined(__MMI_MULTITAP_SPANISH__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_SPANISH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_SPANISH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_SPANISH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_SPANISH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_SPANISH__) */ 

//qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
	case	INPUT_TYPE_MMI_MULTITAP_TAMIL:
			MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_TAMIL;
			wgui_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
			break;
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
	case	INPUT_TYPE_MMI_MULTITAP_BENGALI:
			MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_BENGALI;
			wgui_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
			break;
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
	case	INPUT_TYPE_MMI_MULTITAP_PUNJABI:
			MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_PUNJABI;
			wgui_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
			break;
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
	case	INPUT_TYPE_MMI_MULTITAP_TELUGU:
			MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_TELUGU;
			wgui_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
			break;
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
	case	INPUT_TYPE_MMI_MULTITAP_KANNADA:
			MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_KANNADA;
			wgui_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
			break;
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
	case	INPUT_TYPE_MMI_MULTITAP_MALAYALAM:
			MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_MALAYALAM;
			wgui_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
			break;
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
	case	INPUT_TYPE_MMI_MULTITAP_ORIYA:
			MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_ORIYA;
			wgui_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
			break;
#endif
//qiff add end

#if defined(__MMI_MULTITAP_MARATHI__)
	case	INPUT_TYPE_MMI_MULTITAP_MARATHI:
			MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_MARATHI;
			wgui_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
			break;
#endif

#if defined(__MMI_MULTITAP_URDU__)
	case	INPUT_TYPE_MMI_MULTITAP_URDU:
			MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_URDU;
			wgui_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
			break;
#endif



    #if defined(__MMI_MULTITAP_DANISH__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_DANISH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_DANISH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_DANISH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_DANISH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_DANISH__) */ 

    #if defined(__MMI_MULTITAP_POLISH__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_POLISH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_POLISH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_POLISH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_POLISH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_POLISH__) */ 

    #if defined(__MMI_MULTITAP_FRENCH__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_FRENCH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_FRENCH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_FRENCH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_FRENCH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_FRENCH__) */ 
    #if defined(__MMI_MULTITAP_MYANMAR__)
        case INPUT_TYPE_MMI_MULTITAP_MYANMAR:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_MYANMAR;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif
    #if defined(__MMI_MULTITAP_GERMAN__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_GERMAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_GERMAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_GERMAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_GERMAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_GERMAN__) */ 
    #if defined(__MMI_MULTITAP_ITALIAN__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_ITALIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_ITALIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_ITALIAN__) */ 
    #if defined(__MMI_MULTITAP_RUSSIAN__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_RUSSIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_RUSSIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_RUSSIAN__) */ 

    #if defined(__MMI_MULTITAP_BULGARIAN__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_BULGARIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_BULGARIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_BULGARIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_BULGARIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_BULGARIAN__) */ 

    #if defined(__MMI_MULTITAP_TURKISH__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_TURKISH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_TURKISH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_TURKISH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_TURKISH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_TURKISH__) */ 
    #if defined(__MMI_MULTITAP_PORTUGUESE__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_PORTUGUESE:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_PORTUGUESE:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_PORTUGUESE__) */ 
    #if defined(__MMI_MULTITAP_INDONESIAN__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_INDONESIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_INDONESIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_INDONESIAN__) */ 

    #if defined(__MMI_MULTITAP_CZECH__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_CZECH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_CZECH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_CZECH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_CZECH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_CZECH__) */ 

    #if defined(__MMI_MULTITAP_MALAY__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_MALAY:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_MALAY;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_MALAY:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_MALAY;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_MALAY__) */ 
    #if defined(__MMI_MULTITAP_VIETNAMESE__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_VIETNAMESE:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_VIETNAMESE:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_VIETNAMESE__) */ 

    #if defined(__MMI_MULTITAP_FINNISH__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_FINNISH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_FINNISH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_FINNISH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_FINNISH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_FINNISH__) */ 
    #if defined(__MMI_MULTITAP_HUNGARIAN__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_HUNGARIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_HUNGARIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_HUNGARIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_HUNGARIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_HUNGARIAN__) */ 

    #if defined(__MMI_MULTITAP_HEBREW__)
        case INPUT_TYPE_ALPHANUMERIC_HEBREW:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_HEBREW;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_HEBREW__) */ 

    #if defined(__MMI_MULTITAP_SLOVAK__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_SLOVAK:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_SLOVAK;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_SLOVAK:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_SLOVAK;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_SLOVAK__) */ 
    #if defined(__MMI_MULTITAP_DUTCH__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_DUTCH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_DUTCH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_DUTCH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_DUTCH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_DUTCH__) */ 
    #if defined(__MMI_MULTITAP_ARABIC__)
        case INPUT_TYPE_ALPHANUMERIC_ARABIC:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_ARABIC;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_ARABIC__) */ 

    #if defined(__MMI_MULTITAP_NORWEGIAN__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_NORWEGIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_NORWEGIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_NORWEGIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_NORWEGIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_NORWEGIAN__) */ 

    #if defined(__MMI_MULTITAP_SWEDISH__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_SWEDISH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_SWEDISH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_SWEDISH:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_SWEDISH;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_SWEDISH__) */ 

    #if defined(__MMI_MULTITAP_CROATIAN__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_CROATIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_CROATIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_CROATIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_CROATIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_CROATIAN__) */ 
    #if defined(__MMI_MULTITAP_ROMANIAN__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_ROMANIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_ROMANIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_ROMANIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_ROMANIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_ROMANIAN__) */ 
    #if defined(__MMI_MULTITAP_SLOVENIAN__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_SLOVENIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_SLOVENIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_SLOVENIAN:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_SLOVENIAN;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_SLOVENIAN__) */ 

    #if defined(__MMI_MULTITAP_GREEK__)
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_GREEK:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_UPPERCASE_GREEK;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;

        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_GREEK:
            MMI_current_input_mode = INPUT_MODE_MULTITAP_LOWERCASE_GREEK;
            wgui_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
            break;
    #endif /* defined(__MMI_MULTITAP_GREEK__) */ 

    #if defined(__MMI_T9__) || defined(__MMI_ITAP__)

        case INPUT_TYPE_TR_MULTITAP_BOPOMO:
            MMI_current_input_mode = INPUT_MODE_TR_MULTITAP_BOPOMO;
            wgui_inputbox_information_icon = get_image(WGUI_IME_MULTITAP_BPMF_IMG);
            break;

        case INPUT_TYPE_SM_MULTITAP_PINYIN:
            MMI_current_input_mode = INPUT_MODE_SM_MULTITAP_PINYIN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_MULTITAP_PINYIN_IMG);
            break;

        case INPUT_TYPE_TR_BOPOMO:
            MMI_current_input_mode = INPUT_MODE_TR_BOPOMO;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_BPMF_IMG);
            break;

        case INPUT_TYPE_SM_PINYIN:
            MMI_current_input_mode = INPUT_MODE_SM_PINYIN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_PINYIN_IMG);
            break;

        case INPUT_TYPE_TR_STROKE:
            MMI_current_input_mode = INPUT_MODE_TR_STROKE;
            wgui_inputbox_information_icon = get_image(WGUI_IME_TR_STROKE_IMG);
            break;

        case INPUT_TYPE_SM_STROKE:
            MMI_current_input_mode = INPUT_MODE_SM_STROKE;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SM_STROKE_IMG);
            break;
        case INPUT_TYPE_SMART_UPPERCASE_ABC:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_ABC;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_ABC:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_ABC;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

    #if defined (__MMI_T9_THAI__)
        case INPUT_TYPE_SMART_THAI:
            MMI_current_input_mode = INPUT_MODE_SMART_THAI;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined (__MMI_T9_THAI__) */ 

    #if defined(__MMI_T9_SPANISH__)
        case INPUT_TYPE_SMART_UPPERCASE_SPANISH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_SPANISH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_SPANISH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_SPANISH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_SPANISH__) */ 

    #if defined(__MMI_T9_DANISH__)
        case INPUT_TYPE_SMART_UPPERCASE_DANISH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_DANISH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_DANISH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_DANISH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_DANISH__) */ 

    #if defined(__MMI_T9_POLISH__)
        case INPUT_TYPE_SMART_UPPERCASE_POLISH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_POLISH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_POLISH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_POLISH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_POLISH__) */ 

    #if defined(__MMI_T9_FRENCH__)
        case INPUT_TYPE_SMART_UPPERCASE_FRENCH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_FRENCH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_FRENCH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_FRENCH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_FRENCH__) */ 
    #if defined(__MMI_T9_GERMAN__)
        case INPUT_TYPE_SMART_UPPERCASE_GERMAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_GERMAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_GERMAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_GERMAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_GERMAN__) */ 
    #if defined(__MMI_T9_ITALIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_ITALIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_ITALIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_ITALIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_ITALIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_ITALIAN__) */ 

    #if defined(__MMI_T9_VIETNAMESE__)
        case INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_VIETNAMESE;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_VIETNAMESE;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_VIETNAMESE__) */ 

    #if defined(__MMI_T9_INDONESIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_INDONESIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_INDONESIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_INDONESIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_INDONESIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_INDONESIAN__) */ 

    #if defined(__MMI_T9_PORTUGUESE__)
        case INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_PORTUGUESE;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_PORTUGUESE;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_PORTUGUESE__) */ 

    #if defined(__MMI_T9_CZECH__)
        case INPUT_TYPE_SMART_UPPERCASE_CZECH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_CZECH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_CZECH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_CZECH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_CZECH__) */ 

    #if defined(__MMI_T9_TURKISH__)
        case INPUT_TYPE_SMART_UPPERCASE_TURKISH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_TURKISH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
        case INPUT_TYPE_SMART_LOWERCASE_TURKISH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_TURKISH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_TURKISH__) */ 

    #if defined(__MMI_T9_FINNISH__)
        case INPUT_TYPE_SMART_UPPERCASE_FINNISH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_FINNISH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_FINNISH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_FINNISH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_FINNISH__) */ 
    #if defined(__MMI_T9_HUNGARIAN)
        case INPUT_TYPE_SMART_UPPERCASE_HUNGARIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_HUNGARIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_HUNGARIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_HUNGARIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
    #endif /* defined(__MMI_T9_HUNGARIAN) */ 

    #if defined(__MMI_T9_ARABIC__)
        case INPUT_TYPE_SMART_ARABIC:
            MMI_current_input_mode = INPUT_MODE_SMART_ARABIC;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_ARABIC__) */ 

    #if defined(__MMI_T9_GREEK__)
        case INPUT_TYPE_SMART_UPPERCASE_GREEK:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_GREEK;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_GREEK:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_GREEK;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_GREEK__) */ 
    #if defined(__MMI_T9_HEBREW__)
        case INPUT_TYPE_SMART_HEBREW:
            MMI_current_input_mode = INPUT_MODE_SMART_HEBREW;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_HEBREW__) */ 

    #if defined(__MMI_T9_HINDI__)
        case INPUT_TYPE_SMART_HINDI:
            MMI_current_input_mode = INPUT_MODE_SMART_HINDI;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_HINDI__) */ 
    #if defined(__MMI_T9_NORWEGIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_NORWEGIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_NORWEGIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_NORWEGIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_NORWEGIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_NORWEGIAN__) */ 
    #if defined(__MMI_T9_DUTCH__)
        case INPUT_TYPE_SMART_UPPERCASE_DUTCH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_DUTCH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_DUTCH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_DUTCH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_DUTCH__) */ 
    #if defined(__MMI_T9_BULGARIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_BULGARIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_BULGARIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_BULGARIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_BULGARIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_BULGARIAN__) */ 

    #if defined(__MMI_T9_SLOVENIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_SLOVENIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_SLOVENIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_SLOVENIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_SLOVENIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_SLOVENIAN__) */ 
    #if defined(__MMI_T9_SWEDISH__)
        case INPUT_TYPE_SMART_UPPERCASE_SWEDISH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_SWEDISH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_SWEDISH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_SWEDISH;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_SWEDISH__) */ 

    #if defined(__MMI_T9_CROATIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_CROATIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_CROATIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_CROATIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_CROATIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_CROATIAN__) */ 
    #if defined(__MMI_T9_MALAY__)
        case INPUT_TYPE_SMART_UPPERCASE_MALAY:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_MALAY;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_MALAY:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_MALAY;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_MALAY__) */ 
    #if defined(__MMI_T9_SLOVAK__)
        case INPUT_TYPE_SMART_UPPERCASE_SLOVAK:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_SLOVAK;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_SLOVAK:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_SLOVAK;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_SLOVAK__) */ 
    #if defined(__MMI_T9_ROMANIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_ROMANIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_ROMANIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_ROMANIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_ROMANIAN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_ENGLISH_IMG);
            break;
    #endif /* defined(__MMI_T9_ROMANIAN__) */ 
 
    #elif defined(__MMI_ZI__)
        case INPUT_TYPE_TR_MULTITAP_BOPOMO:
            MMI_current_input_mode = INPUT_MODE_TR_MULTITAP_BOPOMO;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_BPMF_LOGO);
            break;
        case INPUT_TYPE_SM_MULTITAP_PINYIN:
            MMI_current_input_mode = INPUT_MODE_SM_MULTITAP_PINYIN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_PINYIN_LOGO);
            break;
        case INPUT_TYPE_TR_BOPOMO:
            MMI_current_input_mode = INPUT_MODE_TR_BOPOMO;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_BPMF_LOGO);
            break;
        case INPUT_TYPE_SM_PINYIN:
            MMI_current_input_mode = INPUT_MODE_SM_PINYIN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_PINYIN_LOGO);
            break;

        case INPUT_TYPE_TR_STROKE:
            MMI_current_input_mode = INPUT_MODE_TR_STROKE;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_TR_STROKE_LOGO);
            break;

        case INPUT_TYPE_SM_STROKE:
            MMI_current_input_mode = INPUT_MODE_SM_STROKE;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_SM_STROKE_LOGO);
            break;

        case INPUT_TYPE_SMART_UPPERCASE_ABC:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_ABC;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_ABC:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_ABC;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
        case INPUT_TYPE_SMART_THAI:
            MMI_current_input_mode = INPUT_MODE_SMART_THAI;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

    #if defined (__MMI_ZI_HEBREW__)
        case INPUT_TYPE_SMART_HEBREW:
            MMI_current_input_mode = INPUT_MODE_SMART_HEBREW;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined (__MMI_ZI_HEBREW__) */ 

    #if defined(__MMI_ZI_EU_SPANISH__) || defined(__MMI_ZI_SA_SPANISH__)
        case INPUT_TYPE_SMART_UPPERCASE_SPANISH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_SPANISH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_SPANISH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_SPANISH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_EU_SPANISH__) || defined(__MMI_ZI_SA_SPANISH__) */ 

    #if defined(__MMI_ZI_DANISH__)
        case INPUT_TYPE_SMART_UPPERCASE_DANISH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_DANISH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_DANISH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_DANISH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_DANISH__) */ 

    #if defined(__MMI_ZI_POLISH__)
        case INPUT_TYPE_SMART_UPPERCASE_POLISH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_POLISH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_POLISH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_POLISH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_POLISH__) */ 

    #if defined(__MMI_ZI_NORWEGIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_NORWEGIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_NORWEGIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_NORWEGIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_NORWEGIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_NORWEGIAN__) */ 

    #if defined(__MMI_ZI_CA_FRENCH__) || defined(__MMI_ZI_EU_FRENCH__)
        case INPUT_TYPE_SMART_UPPERCASE_FRENCH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_FRENCH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_FRENCH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_FRENCH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_CA_FRENCH__) || defined(__MMI_ZI_EU_FRENCH__) */ 
    #if defined(__MMI_ZI_GERMAN__)
        case INPUT_TYPE_SMART_UPPERCASE_GERMAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_GERMAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_GERMAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_GERMAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_GERMAN__) */ 
    #if defined(__MMI_ZI_ITALIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_ITALIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_ITALIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_ITALIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_ITALIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_ITALIAN__) */ 
    #if defined(__MMI_ZI_RUSSIAN__) || defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_RUSSIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_RUSSIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_RUSSIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_RUSSIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_RUSSIAN__) || defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__) */ 

    #if defined(__MMI_ZI_BULGARIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_BULGARIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_BULGARIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_BULGARIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_BULGARIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_BULGARIAN__) */ 

    #if defined(__MMI_ZI_TURKISH__)
        case INPUT_TYPE_SMART_UPPERCASE_TURKISH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_TURKISH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_TURKISH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_TURKISH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_TURKISH__) */ 
    #if defined(__MMI_ZI_EU_PORTUGUESE__) || defined(__MMI_ZI_SA_PORTUGUESE__) || defined(__MMI_ZI_BZ_PORTUGUESE__)
        case INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_PORTUGUESE;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_PORTUGUESE;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_EU_PORTUGUESE__) || defined(__MMI_ZI_SA_PORTUGUESE__) || defined(__MMI_ZI_BZ_PORTUGUESE__) */ 
    #if defined(__MMI_ZI_INDONESIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_INDONESIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_INDONESIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_INDONESIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_INDONESIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_INDONESIAN__) */ 
    #if defined(__MMI_ZI_MALAY__)
        case INPUT_TYPE_SMART_UPPERCASE_MALAY:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_MALAY;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_MALAY:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_MALAY;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_MALAY__) */ 
    #if defined(__MMI_ZI_VIETNAMESE__)
        case INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_VIETNAMESE;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_VIETNAMESE;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_VIETNAMESE__) */ 

    #if defined(__MMI_ZI_SLOVAK__)
        case INPUT_TYPE_SMART_UPPERCASE_SLOVAK:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_SLOVAK;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_SLOVAK:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_SLOVAK;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_SLOVAK__) */ 
    #if defined(__MMI_ZI_DUTCH__)
        case INPUT_TYPE_SMART_UPPERCASE_DUTCH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_DUTCH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_DUTCH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_DUTCH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_DUTCH__) */ 

    #if defined(__MMI_ZI_SWEDISH__)
        case INPUT_TYPE_SMART_UPPERCASE_SWEDISH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_SWEDISH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_SWEDISH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_SWEDISH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_SWEDISH__) */ 

    #if defined(__MMI_ZI_CROATIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_CROATIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_CROATIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_CROATIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_CROATIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_CROATIAN__) */ 
    #if defined(__MMI_ZI_ROMANIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_ROMANIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_ROMANIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_ROMANIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_ROMANIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_ROMANIAN__) */ 
    #if defined(__MMI_ZI_SLOVENIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_SLOVENIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_SLOVENIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_SLOVENIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_SLOVENIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_SLOVENIAN__) */ 

    #if defined(__MMI_ZI_GREEK__)
        case INPUT_TYPE_SMART_UPPERCASE_GREEK:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_GREEK;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_GREEK:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_GREEK;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_GREEK__) */ 
    #if defined(__MMI_ZI_FINNISH__)
        case INPUT_TYPE_SMART_UPPERCASE_FINNISH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_FINNISH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_FINNISH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_FINNISH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_FINNISH__) */ 
    #if defined(__MMI_ZI_CZECH__)
        case INPUT_TYPE_SMART_UPPERCASE_CZECH:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_CZECH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_CZECH:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_CZECH;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_CZECH__) */ 
    #if defined(__MMI_ZI_HUNGARIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_HUNGARIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_HUNGARIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;

        case INPUT_TYPE_SMART_LOWERCASE_HUNGARIAN:
            MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_HUNGARIAN;
            wgui_inputbox_information_icon = get_image(WGUI_ZI_EZITEXT_LOGO);
            break;
    #endif /* defined(__MMI_ZI_HUNGARIAN__) */ 

    #elif defined(__MMI_KA__)
        case INPUT_TYPE_SM_PINYIN:
            MMI_current_input_mode = INPUT_MODE_SM_PINYIN;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SMART_PINYIN_IMG);
            break;
        case INPUT_TYPE_SM_STROKE:
            MMI_current_input_mode = INPUT_MODE_SM_STROKE;
            wgui_inputbox_information_icon = get_image(WGUI_IME_SM_STROKE_IMG);
            break;
    #endif 
            /* CSD end */
        default:
            MMI_current_input_mode = 2;
            break;
    }

    MMI_current_input_type = input_type;
    flag_use_current_input_type = TRUE;
     
    if (!(item->flags & INLINE_ITEM_OPEN))
    {
        gui_strcpy((UI_string_type) wgui_inline_full_screen_text_edit_buffer, (UI_string_type) i->buffer);
    }

    l = gui_strlen((UI_string_type) wgui_inline_full_screen_text_edit_buffer);
    create_multiline_inputbox_set_buffer((UI_string_type) wgui_inline_full_screen_text_edit_buffer, l, l, 0);

    gui_set_font(MMI_multiline_inputbox.text_font);
    fh = gui_get_character_height();
    inputbox_height = (inputbox_height / (fh + 1)) * (fh + 1);

#if(INSCREEN_MUTLILINE_TITLE_ENABLE)
    move_multiline_inputbox(inputbox_x, inputbox_y + INFORMATION_BAR_HEIGHT + 2);
    resize_multiline_inputbox(inputbox_width, inputbox_height + 4);
    wgui_inputbox_information_flag = 1;
    wgui_inputbox_information_bar_height = INFORMATION_BAR_HEIGHT;
    wgui_setup_input_information(
        inputbox_x + 2,
        inputbox_y + 1,
        inputbox_width - 5,
        wgui_inputbox_information_bar_height);
#else /* (INSCREEN_MUTLILINE_TITLE_ENABLE) */ 
    {
        S32 t_height = 0;

        move_multiline_inputbox(inputbox_x, inputbox_y);
        t_height = (fh + 1) - (INFORMATION_BAR_HEIGHT + 2);
        resize_multiline_inputbox(inputbox_width, inputbox_height + 4 - t_height);
    }
#endif /* (INSCREEN_MUTLILINE_TITLE_ENABLE) */ 
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE |
         UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR);
    show_multiline_inputbox();
}


/*****************************************************************************
 * FUNCTION
 *  wgui_display_inscreen_text_box_input_method
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_display_inscreen_text_box_input_method(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 swidth, sheight, w;
    UI_string_type inputmode_string = (UI_string_type) "";
    stFontAttribute *f = &MMI_small_font;
    color_t c = *current_MMI_theme->shortcut_indicator_text_color;
    UI_filled_area *bg = current_MMI_theme->shortcut_indicator_background_filler;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (MMI_current_input_mode)
    {
        case 0:
            inputmode_string = (UI_string_type) get_string(WGUI_CATEGORY_ABC_STRING_ID);
            break;
        case 1:
            inputmode_string = (UI_string_type) get_string(WGUI_CATEGORY_abc_STRING_ID);
            break;
        case 2:
            inputmode_string = (UI_string_type) get_string(WGUI_CATEGORY_123_STRING_ID);
            break;
    }

    gui_set_font(f);
    gui_measure_string(inputmode_string, &swidth, &sheight);
    w = swidth + 6;
    if (w > UI_device_width)
    {
        w = UI_device_width;
    }
    x1 = x2 - w;
    x2 = x2;
    bg->flags &= ~UI_FILLED_AREA_BORDER;
    gdi_layer_set_clip(x1 + 2, y1 + 2, x2 - 2, y2 - 2);
    gui_draw_filled_area(x1, y1, x2, y2, bg);
    gui_set_text_color(c);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x1 + 3 + swidth, y1 + ((y2 - y1 - sheight) >> 1));
    }
    else
    {
        gui_move_text_cursor(x1 + 3, y1 + ((y2 - y1 - sheight) >> 1));
    }

    gui_set_line_height(sheight);
    gui_print_text(inputmode_string);

    gdi_layer_blt_previous(x1, y1, x2, y2);
}


/*****************************************************************************
 * FUNCTION
 *  inline_multiline_edit_complete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void inline_multiline_edit_complete(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_multiline_complete = 1;
    if (MMI_multi_line_inputbox_present)
    {
        handle_inline_multi_line_edit_complete();
    }
    MMI_multi_line_inputbox_present = 0;
}

extern void wgui_redraw_input_information_background(void);


/*****************************************************************************
 * FUNCTION
 *  gui_draw_inscreen_multi_line_input_box_title
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1      [IN]        
 *  y1      [IN]        
 *  x2      [IN]        
 *  y2      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_draw_inscreen_multi_line_input_box_title(S32 x1, S32 y1, S32 x2, S32 y2)
{
#if(INSCREEN_MULTILINE_SHOW_TITLE)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_inputbox_information_flag)
    {
        wgui_redraw_input_information_background();
    }
#else /* (INSCREEN_MULTILINE_SHOW_TITLE) */ 
    if (wgui_inputbox_information_flag)
    {
        wgui_redraw_input_information_bar();
    }
#endif /* (INSCREEN_MULTILINE_SHOW_TITLE) */ 
}


/*****************************************************************************
 * FUNCTION
 *  gui_break_inline_caption
 * DESCRIPTION
 *  
 * PARAMETERS
 *  buffer              [?]     
 *  charater_length     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_break_inline_caption(U8 *buffer, U8 *charater_length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_buffer_type current_text_p = buffer;
    S32 current_line_width = 0, menu_item_width;
    S32 menu_string_width = gui_get_string_width((UI_string_type) buffer);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    menu_item_width = MMI_content_width - 20;

    if (menu_string_width > menu_item_width)
    {
        UI_character_type current_character = 0;
        S32 character_width, character_height;
        S8 text_gap = current_UI_theme->single_line_input_box_theme->text_gap;
        U8 current_line_character_counter = 0;

        while (current_line_width <= menu_item_width)
        {
            UI_STRING_GET_NEXT_CHARACTER(current_text_p, current_character);
            gui_measure_character(current_character, &character_width, &character_height);
            if (UI_STRING_END_OF_STRING_CHARACTER(current_character))
            {
                break;
            }
            current_line_width += character_width + text_gap;
            current_line_character_counter++;
        }
        *charater_length = current_line_character_counter;
    }
    else
    {
        *charater_length = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  wgui_show_inline_multi_line_rd_only
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x1          [IN]        
 *  y1          [IN]        
 *  width       [IN]        
 *  height      [IN]        
 *  item        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_show_inline_multi_line_rd_only(S32 x1, S32 y1, S32 width, S32 height, wgui_inline_item *item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) item->item;
    S32 l;

    S32 inputbox_x = x1;
    S32 inputbox_y = y1;
    S32 inputbox_width = width;
    S32 inputbox_height = height * i->max_lines;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_current_inline_item_type == INLINE_ITEM_TYPE_FULL_SCREEN_EDIT)
    {
        return;
    }
    l = gui_strlen((UI_string_type) i->buffer);
    create_multiline_inputbox_set_buffer((UI_string_type) i->buffer, l, l, 0);
    move_multiline_inputbox(inputbox_x, inputbox_y);
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE |
         UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR);

    resize_multiline_inputbox(inputbox_width, inputbox_height);
    show_multiline_inputbox();
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineMultiLineRdOnly
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item            [?]         
 *  buffer          [?]         
 *  buffer_size     [IN]        
 *  input_type      [IN]        
 *  max_lines       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineMultiLineRdOnly(InlineItem *item, U8 *buffer, S32 buffer_size, U16 input_type, U8 max_lines)    /* input_type :- Other items are hilightable or not. */
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (n_inline_text_edit_items >= MAX_INLINE_TEXT_EDIT_ITEMS)
    {
        return;
    }
    item->item = (void*)&wgui_inline_text_edit_items[n_inline_text_edit_items];
    item->flags = INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY;
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_VIEW_MODE;

    n_inline_text_edit_items++;

    i = (wgui_inline_item_text_edit*) item->item;
    i->max_lines = max_lines;
    i->title = 0;
    i->title_icon = 0;
    i->buffer = buffer;
    i->buffer_size = buffer_size;
    i->flags = 0;
    i->input_type = input_type;

    item->text_p = i->buffer;
    item->handler = handle_inline_multi_line_rd_only;

    i->edit_complete_callback = NULL;
    i->custom_setup_function = NULL;
    i->validation_callback = NULL;

    i->UCS2_count = -1;
    MMI_multi_line_inputbox_present = 2;
}


/*****************************************************************************
 * FUNCTION
 *  menu_item_inline_multiline_box
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 menu_item_inline_multiline_box()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((wgui_inline_items[MMI_fixed_list_menu.n_items - 1].flags & 0xff) == INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  redraw_multiline_rd_only
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_multiline_rd_only(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_multiline_inputbox_viewer_keys();
    show_multiline_inputbox();
}


/*****************************************************************************
 * FUNCTION
 *  handle_multi_line_rd_only_close
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void handle_multi_line_rd_only_close(void)
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


/*****************************************************************************
 * FUNCTION
 *  inscreen_multi_line_input_box_changed
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 inscreen_multi_line_input_box_changed()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return inscreen_multi_line_change_flag;
}


/*****************************************************************************
 * FUNCTION
 *  set_inscreen_multi_line_input_box_changed
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void set_inscreen_multi_line_input_box_changed()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inscreen_multi_line_change_flag = 1;
}


/*****************************************************************************
 * FUNCTION
 *  reset_inscreen_multi_line_input_box_changed
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_inscreen_multi_line_input_box_changed()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inscreen_multi_line_change_flag = 0;
}


/*****************************************************************************
 * FUNCTION
 *  SetInlineListMenuChanged
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SetInlineListMenuChanged(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_wgui_inline_list_menu_changed();
}


/*****************************************************************************
 * FUNCTION
 *  GetInlineListMenuChanged
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 GetInlineListMenuChanged(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return wgui_inline_list_menu_changed;
}


/*****************************************************************************
 * FUNCTION
 *  handle_inline_multi_line_rd_only
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  key_code            [IN]        
 *  key_event           [IN]        
 *  text_p              [IN]        
 *  item                [?]         
 *  flags               [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void handle_inline_multi_line_rd_only(
        S32 x,
        S32 y,
        S32 width,
        S32 height,
        S32 key_code,
        S32 key_event,
        U8 **text_p,
        void *item,
        U32 flags,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    wgui_inline_item_text_edit *i = (wgui_inline_item_text_edit*) item;
    S32 l = 0;

    S32 inputbox_x = x;
    S32 inputbox_y = y;
    S32 inputbox_width = width;
    S32 inputbox_height = height * (i->max_lines);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_current_inline_item_type = INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY;

    l = gui_strlen((UI_string_type) i->buffer);
    if (history_buffer == NULL || inscreen_multi_line_input_box_changed())
    {
        create_multiline_inputbox_set_buffer((UI_string_type) i->buffer, l, l, 0);
        reset_inscreen_multi_line_input_box_changed();
    }
    else
    {
        create_multiline_inputbox_set_buffer_no_text_reset((UI_string_type) i->buffer, l, l, 0);
    }

    move_multiline_inputbox(inputbox_x, inputbox_y);
    MMI_multiline_inputbox.flags |=
        (UI_MULTI_LINE_INPUT_BOX_AUTO_VERTICAL_SCROLL | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE |
         UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_INSIDE_INLINE_MENU
         /* |UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW */ );

    resize_multiline_inputbox(inputbox_width, inputbox_height);
    register_multiline_inputbox_viewer_keys();

    show_multiline_inputbox();
    redraw_current_inline_item = redraw_multiline_rd_only;
    complete_inline_item_edit = handle_multi_line_rd_only_close;
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  UI_dummy_inline_item_pen_function
 * DESCRIPTION
 *  
 * PARAMETERS
 *  pen_event       [IN]        
 *  x               [IN]        
 *  y               [IN]        
 *  item_event      [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL UI_dummy_inline_item_pen_function(
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_inline_item_pen_enum *item_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(pen_event);
    UI_UNUSED_PARAMETER(x);
    UI_UNUSED_PARAMETER(y);
    *item_event = GUI_INLINE_ITEM_PEN_NONE;
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  gui_inline_handle_pen_event_by_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m                   [?]         
 *  pen_event           [IN]        
 *  x                   [IN]        
 *  y                   [IN]        
 *  inline_event        [?]         
 * RETURNS
 *  
 *****************************************************************************/
static BOOL gui_inline_handle_pen_event_by_item(
                fixed_list_menu *m,
                mmi_pen_event_type_enum pen_event,
                S16 x,
                S16 y,
                gui_inline_pen_enum *inline_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_inline_item_pen_enum item_event;
    BOOL ret;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(m);

    *inline_event = GUI_INLINE_PEN_NONE;

    ret = wgui_current_inline_item_pen_function(pen_event, x, y, &item_event);
    if (ret)
    {
        if (item_event == GUI_INLINE_ITEM_PEN_NEED_REDRAW_MENU)
        {
            *inline_event = GUI_INLINE_PEN_NEED_REDRAW;
        }
        else if (item_event == GUI_INLINE_ITEM_PEN_NEED_REDRAW_ITEM)
        {
            redraw_current_inline_item();
        }
        else if (item_event == GUI_INLINE_ITEM_PEN_SELECTED)
        {
            *inline_event = GUI_INLINE_PEN_ITEM_SELECTED;
        }
    }

    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  gui_inline_scroll_by_pen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m                   [?]         
 *  x                   [IN]        
 *  y                   [IN]        
 *  first_displayed     [IN]        
 *  inline_event        [?]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_inline_scroll_by_pen(
                fixed_list_menu *m,
                S16 x,
                S16 y,
                S32 first_displayed,
                gui_inline_pen_enum *inline_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 last_displayed;
    S32 iwidth, iheight;
    S32 total_height, i;

    MMI_BOOL history_stored;
    U8 gui_buffer[MAX_GUI_BUFFER];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (first_displayed == m->first_displayed_item)
    {
        *inline_event = GUI_INLINE_PEN_NONE;
        return;
    }

    /* 
     * Sometimes the inline editor is scrolled (m->first_displayed is changed) but
     * the highlighted item (m->highlighted_item) stays unchanged.
     * 
     * In this case, we should keep the context of inline item unchanged 
     * (e.g. editor cursor & input method), and we put the information in gui_buffer[].
     *
     * Reference: wgui_get_inline_history(), wgui_inline_edit_get_current_item_history(), 
     * and wgui_current_history_buffer.
     */
    history_stored = MMI_TRUE;
    switch (wgui_current_inline_item_type)
    {
        case INLINE_ITEM_TYPE_TEXT_EDIT:
            inline_text_edit_get_history(gui_buffer, MMI_current_input_type);
            break;

        case INLINE_ITEM_TYPE_DATE:
            inline_date_edit_get_history(gui_buffer);
            break;

        case INLINE_ITEM_TYPE_TIME:
            inline_time_edit_get_history(gui_buffer);
            break;

        case INLINE_ITEM_TYPE_TIME_PERIOD:
            inline_time_period_edit_get_history(gui_buffer);
            break;

        case INLINE_ITEM_TYPE_IP4_EDIT:
            inline_IP4_edit_get_history(gui_buffer);
            break;

        case INLINE_ITEM_TYPE_DOW_SELECT:
            inline_DOW_select_get_history(gui_buffer);
            break;

        case INLINE_ITEM_TYPE_MULTI_LINE_EDIT:
            inline_multi_line_edit_get_history(gui_buffer, MMI_current_input_type);
            break;

        default:
            history_stored = MMI_FALSE;
    }

    /* Finalize current editing. */
    complete_inline_item_edit();

    /*
     * Note that m->first_displayed_item might be changed inside m->item_highlighted() 
     * (which is typically wgui_inline_edit_default_list_highlight_handler())
     * if INLINE_ITEM_FOLLOW_DOWN or INLINE_ITEM_FOLLOW_UP is set on the highlighed item. 
     * 
     * inline item handler is invoked from handle_inline_item_location() (typically from gui_show_fixed_list_menu())
     */

    *inline_event = GUI_INLINE_PEN_NONE;

    /* Scroll down */
    if (first_displayed > m->highlighted_item)
    {
        m->flags |= UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
        inline_fixed_list_goto_item(first_displayed, MMI_FALSE);
        m->flags &= ~UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
    }
    else
    {
        /* Compute last_displayed */
        last_displayed = m->n_items - 1;
        total_height = 0;
        for (i = first_displayed; i < m->n_items; i++)
        {
            m->item_measure_function(m->items[i], m->common_item_data, &iwidth, &iheight);
            total_height += iheight;
            if (total_height > m->height + 1)
            {
                last_displayed = i - 1;
                break;
            }
        }

        /* Scroll up */
        if (last_displayed < m->highlighted_item)
        {
            m->flags |= UI_LIST_MENU_LAST_SHIFT_HIGHLIGHTED_ITEM;
            inline_fixed_list_goto_item(last_displayed, MMI_TRUE);
            m->flags &= ~UI_LIST_MENU_LAST_SHIFT_HIGHLIGHTED_ITEM;
        }
        else
        {
            S32 old_first_displayed;

            old_first_displayed = m->first_displayed_item;

            if (history_stored)
            {
                /* 
                 * Restore the context of the original highlighted item.
                 *
                 * Ex: Assume it is an inline text edit, we need to setup history
                 * buffer here; otherwise the virtual keyboard type is reset,
                 * and the list menu height might be changed.                     
                 *
                 * However, the current code logic assumes that menu height should
                 * stay unchanged after m->item_highlighted(). 
                 */
                wgui_current_history_buffer = gui_buffer;
			}
            m->item_unhighlighted(m->highlighted_item);
            m->first_displayed_item = first_displayed;
            m->last_displayed_item = last_displayed;
            m->item_highlighted(m->highlighted_item);

            /* 
             * m->item_highlighted() might change m->first_displayed_item again if 
             * a. INLINE_ITEM_FOLLOW_DOWN and INLINE_ITEM_FOLLOW_UP is set to highlighted item.
             * b. m->first_displayed_item is incremented to display inline caption item
             *
             * Usually we can test the scenario in MMI Todo screen.
             */
            if (m->first_displayed_item == old_first_displayed)
            {
                /* 
                 * In this case, m->first_displayed_item != first_displayed 
                 * because there are other "restrictions" that set 
                 * m->first_displayed_item to the original value.
                 *
                 * It happens in many inline editor screens in MMI. 
                 */
                if (first_displayed < m->first_displayed_item)
                {
                    /*
                     * In this case, we want to scroll up the menu but fail to do so.
                     *
                     * For example, the last displayed item is currently highlighted, and 
                     * the item before the last displayed item is an inline caption.
                     * If we scroll up by one item, The inline caption is highlighted although
                     * it cannot be highlighted, and then highlight is set to the next 
                     * inline item, which is the original highlighted one. 
                     * As a result, the inline editor is not scrolled at all.
                     * 
                     * In order to solve this, we scroll "one" more inline item up.
                     */
                    m->flags |= UI_LIST_MENU_LAST_SHIFT_HIGHLIGHTED_ITEM;
                    inline_fixed_list_goto_item(last_displayed - 1, MMI_TRUE);
                    m->flags &= ~UI_LIST_MENU_LAST_SHIFT_HIGHLIGHTED_ITEM;
                }
                else
                {
                    /* Similar to previous case */
                    m->flags |= UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
                    inline_fixed_list_goto_item(first_displayed + 1, MMI_FALSE);
                    m->flags &= ~UI_LIST_MENU_FIRST_SHIFT_HIGHLIGHTED_ITEM;
                }
            }
            else
            {
                gui_lock_double_buffer();

                redraw_fixed_list();
                redraw_current_inline_item();
                /* 
                 * Redraw virtual keyboard. 
                 * There is a small gap (e.g. 2 pixels) between fixed list and virtual keyboard
                 * and some inline item draw outside the list, we need to clear the gap by
                 * redrawing virtual keyboard.
                 */
                if (GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY != MMI_virtual_keyboard.lang_type)
                {
                    mmi_pen_editor_clear_and_show_virtual_keyboard_area();
                }
                gui_unlock_double_buffer();
                gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
            }

            if (history_stored)
            {
                /* Failsafe step because 'gui_buffer' is local variable. 
                   and it's lifecyle is inside the current function */
                wgui_current_history_buffer = NULL;
            }
        }
    }

    /* Because we do not always follow the value of 'first_displayed',
       we need to adjust scrollbar again. */
    gui_vertical_scrollbar_reconfigure_pen_value(&m->vbar, x, y);
}


/*****************************************************************************
 * FUNCTION
 *  gui_inline_translate_pen_event
 * DESCRIPTION
 *  
 * PARAMETERS
 *  m                   [?]         
 *  pen_event           [IN]        
 *  x                   [IN]        
 *  y                   [IN]        
 *  inline_event        [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL gui_inline_translate_pen_event(
        fixed_list_menu *m,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_inline_pen_enum *inline_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret = MMI_TRUE;
    gui_scrollbar_pen_enum scrollbar_event;
    gui_pen_event_param_struct scrollbar_param;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *inline_event = GUI_INLINE_PEN_NONE;

    if (pen_event == MMI_PEN_EVENT_DOWN)
    {
        wgui_inline_pen_down_item_index = -1;

        if (PEN_CHECK_BOUND(x, y, m->x, m->y, m->width, m->height))
        {
            if (wgui_current_inline_item_type == INLINE_ITEM_TYPE_FULL_SCREEN_EDIT)
            {
                /* Redirect pen events to editor */
                wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_FULLSCREEN_EDITOR;
                /* Handled by editor */
            }
            else if (!(m->flags & UI_LIST_MENU_DISABLE_SCROLLBAR) &&
                     gui_vertical_scrollbar_translate_pen_event(
                        &m->vbar,
                        pen_event,
                        x,
                        y,
                        &scrollbar_event,
                        &scrollbar_param))
            {
                wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_SCROLLBAR;
                if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
                {
                    gui_inline_scroll_by_pen(m, x, y, scrollbar_param._u.i, inline_event);
                }
            }
            else
            {
                S32 item_index;
                BOOL is_pen_inside_menu;

                is_pen_inside_menu = gui_fixed_list_menu_translate_pen_position(m, y, &item_index);

                wgui_inline_pen_down_item_index = item_index;

            #ifdef __MMI_INSCREEN_MULTILINE_TEXTBOX__
                /* Special case: inline multiline is larger than its corresponding menu item */
                if (( !is_pen_inside_menu &&
                    item_index == m->n_items - 1 &&
                    (wgui_inline_items[item_index].flags & 0xff) == INLINE_ITEM_TYPE_MULTI_LINE_EDIT) ||
                    (wgui_inline_items[item_index].flags & 0xff) == INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY)
                {
                    is_pen_inside_menu = MMI_TRUE;
                }
            #endif /* __MMI_INSCREEN_MULTILINE_TEXTBOX__ */ 

                if (!is_pen_inside_menu)
                {
                    wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_NONE;
                }
                else if (item_index != m->highlighted_item)
                {
                    if ((wgui_inline_items[item_index].flags & 0xff) == INLINE_ITEM_TYPE_CAPTION &&
                        item_index == MMI_fixed_list_menu.n_items - 1)
                    {
                        /* Clicking on the last item that is inline caption. We should not switch to it; otherwise, it goes to the first item */
                        wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_NONE;
                    }
                    else if ((wgui_inline_items[item_index].flags & 0xff) == INLINE_ITEM_TYPE_CAPTION &&
                             item_index == MMI_fixed_list_menu.n_items - 2 &&
                             (wgui_inline_items[item_index + 1].flags & INLINE_ITEM_DISABLED))
                    {
                        /* The last item is disabled. Clicking on the inline caption before it */
                        wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_NONE;
                    }
                    else if (!(wgui_inline_items[item_index].flags & INLINE_ITEM_DISABLED))
                    {
                        wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_SWITCH_ITEM;
                    }
                    else
                    {
                        wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_NONE;
                    }
                }
                else
                {
                    /*
                     * Because the inline item (e.g. horizontal select) might be smaller than the menu item  
                     * * the pen handler might return MMI_FALSE.
                     * *
                     * * Note: It can be extended to handle SetInlineItemActivation() (for fullscreen editor), 
                     * * but we do not handle it deliberately.
                     */
                    if (gui_inline_handle_pen_event_by_item(m, pen_event, x, y, inline_event))
                    {
                        wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_CURRENT_ITEM;
                    }
                    else
                    {
                        /*
                         * After long press animation, invoke left softkey handler. 
                         * * Tricky: it only apply to certain inline item types.
                         */
                        S32 type = wgui_inline_items[wgui_inline_item_highlighted_index].flags & 0xff;

                        if (type == INLINE_ITEM_TYPE_FULL_SCREEN_EDIT ||
                            type == INLINE_ITEM_TYPE_DISPLAY_ONLY ||
                            type == INLINE_ITEM_IMAGE_TEXT_TYPE || type == INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY)
                        {
                            gui_wait_longpress_enum w;

                            w = gui_pen_wait_longpress(MMI_PEN_EVENT_DOWN, x, y);
                            if (w == GUI_WAIT_LONGPRESS_INVALID)
                            {
                                wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_NONE;
                            }
                            else
                            {
                                wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_WAIT_LONGPRESS;
                            }
                        }
                        else
                        {
                            wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_NONE;
                        }
                    }
                }
            }
        }
        else
        {
            ret = MMI_FALSE;
        }
    }
    else
    {
        if (wgui_inline_pen_handler_type == WGUI_INLINE_PEN_HANDLER_SCROLLBAR)
        {
            gui_vertical_scrollbar_translate_pen_event(&m->vbar, pen_event, x, y, &scrollbar_event, &scrollbar_param);
            if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
            {
                gui_inline_scroll_by_pen(m, x, y, scrollbar_param._u.i, inline_event);
            }
        }
        else if (wgui_inline_pen_handler_type == WGUI_INLINE_PEN_HANDLER_WAIT_LONGPRESS)
        {
            gui_wait_longpress_enum w;

            w = gui_pen_wait_longpress(pen_event, x, y);
            if (w == GUI_WAIT_LONGPRESS_INVALID)
            {
                wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_NONE;
            }
            else if (w == GUI_WAIT_LONGPRESS_READY)
            {
                *inline_event = GUI_INLINE_PEN_ITEM_SELECTED;
                /* ignore subsequent pen events */
                wgui_inline_pen_handler_type = WGUI_INLINE_PEN_HANDLER_NONE;
            }
        }
        else if (wgui_inline_pen_handler_type == WGUI_INLINE_PEN_HANDLER_CURRENT_ITEM)
        {
            gui_inline_handle_pen_event_by_item(m, pen_event, x, y, inline_event);
        }
        else if (wgui_inline_pen_handler_type == WGUI_INLINE_PEN_HANDLER_FULLSCREEN_EDITOR)
        {
            /* Handled by editor */
        }
        else if (wgui_inline_pen_handler_type == WGUI_INLINE_PEN_HANDLER_SWITCH_ITEM)
        {
            if (pen_event == MMI_PEN_EVENT_UP)
            {
                S32 item_index;

                gui_fixed_list_menu_translate_pen_position(m, y, &item_index);
                if (item_index == wgui_inline_pen_down_item_index)
                {
                    complete_inline_item_edit();    /* Finalize current editing */
                    /* Automatically select the next item if it's inline caption item */
                    inline_fixed_list_goto_item(item_index, MMI_FALSE);
                }
            }
        }
        else if (wgui_inline_pen_handler_type == WGUI_INLINE_PEN_HANDLER_NONE)
        {
            /* Do nothing */
        }
    }
    return ret;
}

#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCommonCategoryInlineEdit
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  number_of_items         [IN]        
 *  list_of_icons           [?]         
 *  list_of_items           [?]         
 *  highlighted_item        [IN]        
 *  history_buffer          [?]         
 *  highlight_handler       [IN]        
 *  unhighlight_handler     [IN]        
 *  history_id              [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCommonCategoryInlineEdit(
        UI_string_type title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S32 number_of_items,
        U16 *list_of_icons,
        InlineItem *list_of_items,
        S32 highlighted_item,
        U8 *history_buffer,
        void (*highlight_handler) (S32 item_index),
        void (*unhighlight_handler) (S32 item_index),
        U16 history_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 h_flag;
    S32 i;
    U16 inline_item_index = 0;

    UI_fixed_icontext_menuitem_theme *saved_themedata1 = current_fixed_icontext_menuitem_theme;
    UI_fixed_list_menu_theme *saved_themedata2 = current_fixed_list_menu_theme;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Backup font */
    cat57_f1 = MMI_fixed_icontext_menuitem.text_font;
    cat57_f2 = MMI_fixed_text_menuitem.text_font;

#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_block_list_effect();
#endif 

    MMI_menu_shortcut_number = -1;
    clear_category_screen_key_handlers();
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();

    MMI_title_string = title;
    MMI_title_icon = (PU8) get_image(title_icon);

    current_fixed_icontext_menuitem_theme = &wgui_inline_fixed_icontext_menuitem_theme;
    current_fixed_list_menu_theme = &wgui_inline_fixed_list_menu_theme;

    gui_set_fixed_list_menu_current_theme(&MMI_fixed_list_menu);

    create_fixed_icontext_menuitems();
    associate_fixed_icontext_list();
#ifdef __MMI_TOUCH_SCREEN__
    /* Avoid menu position shifted when it is resized for differet virtual keyboard layout */
    MMI_fixed_list_menu.flags |= UI_LIST_MENU_ALIGN_TO_TOP;
#endif /* __MMI_TOUCH_SCREEN__ */ 

#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
    temp_inline_edit_normal_filler.border_color = current_fixed_icontext_menuitem_theme->focussed_filler->c;
    MMI_fixed_icontext_menuitem.selected_filler = &temp_inline_edit_normal_filler;
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 

#ifdef __MMI_UI_TRANSPARENT_EFFECT__
    disable_fixed_icontext_menuitem_transparent_effect();
#endif 
    if (list_of_icons == NULL)
    {
        /* without icons */
        for (i = 0; (i < number_of_items) && (i < MAX_INLINE_ITEMS); i++)
        {
            wgui_inline_items[i] = list_of_items[i];
            add_fixed_icontext_item((UI_string_type) wgui_inline_items[i].text_p, NULL);

            /* 112905 user inline select Start */
            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_USER_DEFINED_SELECT)
            {
                MMI_fixed_icontext_menuitems[i].ext_flags |= UI_MENUITEM_EXT_DISABLE_FOCUSSED_TEXT_DISPLAY;
            }
            /* 112905 user inline select End */

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_DOW_SELECT)
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_COLOR_SELECT)
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
            }

            if (wgui_inline_items[i].flags & INLINE_ITEM_DISABLE_HIGHLIGHT)
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS;
            }

            if ((wgui_inline_items[i].flags & INLINE_ITEM_BOUNDARY_MASK) == INLINE_ITEM_BOUNDARY_ALWAYS)
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_IMAGE_TEXT_TYPE)
            {
                if (wgui_inline_items[i].show_img_flag == TRUE)
                {
                    MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
                }

            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_ATTACH_IMAGE)
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
            }

            if (wgui_inline_items[i].flags & INLINE_ITEM_DOTTED_UNDERLINE)
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_DOTTED_UNDERLINE;
            }

            if (((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY) ||
                ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_MULTI_LINE_EDIT))
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_TIME_PERIOD)
            {
                MMI_fixed_icontext_menuitems[i].ext_flags |= UI_MENUITEM_EXT_DISABLE_FOCUSSED_TEXT_DISPLAY;
            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_SELECT)
            {
                if (wgui_inline_items[i].show_img_flag == TRUE)
                {
                    MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
                }

                /* flags must be enabled in all the cases so that inline select item will have
                   similar behaviour in each case. */
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS;
                MMI_fixed_icontext_menuitems[i].ext_flags |= UI_MENUITEM_EXT_DISABLE_FOCUSSED_TEXT_DISPLAY;

            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_COLOR_SELECT)
            {
                if (wgui_inline_items[i].show_img_flag == TRUE)
                {
                    MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
                }
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS;
                MMI_fixed_icontext_menuitems[i].ext_flags |= UI_MENUITEM_EXT_DISABLE_FOCUSSED_TEXT_DISPLAY;
            }

        #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
            if ((((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_TEXT_EDIT)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_FULL_SCREEN_EDIT)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_DATE)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_SELECT)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_TIME)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_TIME_PERIOD)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_DOW_SELECT)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_IP4_EDIT)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_COLOR_SELECT))
                && (IsInlineItemDefaultTextEnable()))
            {
                MMI_fixed_icontext_menuitems[i].ext_flags |= UI_MENUITEM_EXT_SHOW_IN_ROUNDED_RECT;
            }
        #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 
        }
    }
    else
    {
        /* with icons */
        for (i = 0; (i < number_of_items) && (i < MAX_INLINE_ITEMS); i++)
        {
            wgui_inline_items[i] = list_of_items[i];
            wgui_inline_items[i].side_img = get_image(list_of_icons[i]);

#if !defined(MMI_ULC)
            add_fixed_icontext_item(
                (UI_string_type) wgui_inline_items[i].text_p,
                wgui_get_list_menu_icon_if_not_empty(inline_item_index, list_of_icons[i]));
			if (wgui_inline_items[i].side_img != NULL)
			{
				inline_item_index++;
			}
#else
			add_fixed_icontext_item(
				(UI_string_type) wgui_inline_items[i].text_p,
				wgui_get_list_menu_icon(i, list_of_icons[i]));
#endif

            if (wgui_inline_items[i].side_img != NULL)
            {
                inline_item_index++;
            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_USER_DEFINED_SELECT)
            {
                MMI_fixed_icontext_menuitems[i].ext_flags |= UI_MENUITEM_EXT_DISABLE_FOCUSSED_TEXT_DISPLAY;
            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_DOW_SELECT)
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_COLOR_SELECT)
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
            }

            if (wgui_inline_items[i].flags & INLINE_ITEM_DISABLE_HIGHLIGHT)
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS;
            }

            if ((wgui_inline_items[i].flags & INLINE_ITEM_BOUNDARY_MASK) == INLINE_ITEM_BOUNDARY_ALWAYS)
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
            }

            if (((wgui_inline_items[i].flags & INLINE_ITEM_LEFT_ALIGN) == INLINE_ITEM_LEFT_ALIGN) &&
                ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_CAPTION) &&
                (get_image(list_of_icons[i]) == NULL))
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_DISABLE_ICON;
            }

            if (wgui_inline_items[i].flags & INLINE_ITEM_DOTTED_UNDERLINE)
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_DOTTED_UNDERLINE;
            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_IMAGE_TEXT_TYPE)
            {
                if (wgui_inline_items[i].show_img_flag == TRUE)
                {
                    MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
                }

            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_ATTACH_IMAGE)
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
            }

            if (((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_MULTI_LINE_RD_ONLY) ||
                ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_MULTI_LINE_EDIT))
            {
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_TIME_PERIOD)
            {
                MMI_fixed_icontext_menuitems[i].ext_flags |= UI_MENUITEM_EXT_DISABLE_FOCUSSED_TEXT_DISPLAY;
            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_SELECT)
            {
                if (wgui_inline_items[i].show_img_flag == TRUE)
                {
                    MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
                }
                /* flags must be enabled in all the cases so that inline select item will have
                   similar behaviour in each case. */
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS;
                MMI_fixed_icontext_menuitems[i].ext_flags |= UI_MENUITEM_EXT_DISABLE_FOCUSSED_TEXT_DISPLAY;
            }

            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_COLOR_SELECT)
            {
                if (wgui_inline_items[i].show_img_flag == TRUE)
                {
                    MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_INLINE_EDIT_OBJECT;
                }
                MMI_fixed_icontext_menuitems[i].flags |= UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS;
                MMI_fixed_icontext_menuitems[i].ext_flags |= UI_MENUITEM_EXT_DISABLE_FOCUSSED_TEXT_DISPLAY;
            }

        #ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__
            if ((((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_TEXT_EDIT)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_FULL_SCREEN_EDIT)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_DATE)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_SELECT)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_TIME)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_TIME_PERIOD)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_DOW_SELECT)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_IP4_EDIT)
                 || ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_COLOR_SELECT))
                && (IsInlineItemDefaultTextEnable()))
            {
                MMI_fixed_icontext_menuitems[i].ext_flags |= UI_MENUITEM_EXT_SHOW_IN_ROUNDED_RECT;
            }
        #endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 

        }
    }
    wgui_setup_inline_edit_themes();
    wgui_n_inline_items = number_of_items;
    register_inline_fixed_list_keys();
    //resize_fixed_icontext_menuitems(0, MMI_MENUITEM_HEIGHT);
    resize_fixed_icontext_menuitems(0,get_menu_item_height());
    if (list_of_icons == NULL)
    {
        set_fixed_icontext_positions(2, 0, 1, 0);
    }
    else
    {
        set_fixed_icontext_positions(MMI_INLINE_EDIT_MENUITEM_HEIGHT + 2, 0, 1, 0);
    }

    /* Inline screens use Pop up description components to get x,y,width,height   */
#if(UI_ENABLE_POP_UP_DESCRIPTIONS)
    gui_display_pop_up_description = handle_inline_item_location;
#ifdef __MMI_UI_HINTS_IN_MENUITEM__
    gui_get_current_pop_up_description_string = UI_dummy_get_current_pop_up_description_string;
#endif 
#endif /* (UI_ENABLE_POP_UP_DESCRIPTIONS) */ 

    register_fixed_list_highlight_handler(highlight_handler);
    register_fixed_list_unhighlight_handler(unhighlight_handler);

    inline_edit_current_highlight_handler = highlight_handler;

    /* Override default font */
    MMI_fixed_icontext_menuitem.text_font = &MMI_medium_font;
    MMI_fixed_text_menuitem.text_font = &MMI_medium_font;

    MMI_disable_title_shortcut_display = 1;
    MMI_current_menu_type = LIST_MENU;
    h_flag = set_list_menu_category_history(history_id, history_buffer);
    if (h_flag)
    {
        U32 *flags;
        S32 s = sizeof(list_menu_category_history);

        s = (s + 3) / 4;
        s *= 4;
        flags = (U32*) (history_buffer + s);

        wgui_inline_list_menu_changed = (U8) * flags;
        wgui_current_history_buffer = (U8*) (history_buffer + s + sizeof(U32));
        fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);

    #if defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__)
        for (i = 0; (i < number_of_items) && (i < MAX_INLINE_ITEMS); i++)
        {
            if ((wgui_inline_items[i].flags & 0xff) == INLINE_ITEM_TYPE_MULTI_LINE_EDIT)
            {
                wgui_inline_items[i].flags |= INLINE_ITEM_OPEN;
            }
        }
    #endif /* defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__) */ 
    }
    else
    {
        wgui_current_history_buffer = NULL;
        inline_image_text_edit_flag = 0;
        fixed_list_goto_item_no_redraw(highlighted_item);
    }
    current_fixed_icontext_menuitem_theme = saved_themedata1;
    current_fixed_list_menu_theme = saved_themedata2;

}


/*****************************************************************************
 * FUNCTION
 *  RegisterInlineItemChangedNotification
 * DESCRIPTION
 *  Register inline item changed notification callback
 * PARAMETERS
 *  callback        [IN]        Callback function to application
 * RETURNS
 *  void
 *****************************************************************************/
void RegisterInlineItemChangedNotification(void (*callback) (U16 index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_item_changed_handler = callback;
}


/*****************************************************************************
 * FUNCTION
 *  ResetInlineItemChangedNotification
 * DESCRIPTION
 *  Reset inline item changed notification callback
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ResetInlineItemChangedNotification(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    inline_item_changed_handler = NULL;
}

#endif /* defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__) */ 

#ifdef __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__


/*****************************************************************************
 * FUNCTION
 *  inline_item_default_text_show_background
 * DESCRIPTION
 *  show the background of the inline menu item in case of
 *  default text effect.
 * PARAMETERS
 *  x1      [IN]        Start x position
 *  y1      [IN]        Start y position
 *  x2      [IN]        End x position
 *  y2      [IN]        End y position
 * RETURNS
 *  void
 *****************************************************************************/
void inline_item_default_text_show_background(S32 x1, S32 y1, S32 x2, S32 y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gui_set_clip(x1, y1, x2, y2);
    gui_draw_filled_area(x1, y1, x2, y2, &temp_inline_edit_normal_filler);
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  inline_text_edit_show_background
 * DESCRIPTION
 *  show the border of the inline text edit menu item with 3D
 *  effect.
 * PARAMETERS
 *  b       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void inline_text_edit_show_background(single_line_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2;
    color_t border_color1, border_color2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    border_color1 = gui_blend_two_color(b->normal_filler->c, gui_color(255, 255, 255), 60, 100 - 60);
    border_color2 = gui_blend_two_color(b->normal_filler->c, gui_color(0, 0, 0), 75, 100 - 75);

    gdi_layer_reset_clip();
    x1 = b->x - 2;
    y1 = b->y - 2;
    x2 = b->x + b->width + 1;
    y2 = b->y + b->height + 1;

    /* draw the whitish rectangle */
    gui_draw_rectangle(x1, y1, x2 - 1, y2 - 1, border_color1);

    /* draw the top left dark border */
    gui_draw_horizontal_line(x1 + 1, x2 - 2, y1 + 1, border_color2);
    gui_draw_vertical_line(y1 + 1, y2 - 2, x1 + 1, border_color2);

    /* draw the bottom right highlight color border */
    gui_draw_horizontal_line(x1 + 1, x2, y2, b->normal_filler->c);
    gui_draw_vertical_line(y1 + 1, y2, x2, b->normal_filler->c);
}
#endif /* __MMI_UI_INLINE_EDIT_DEFAULT_TEXT_EFFECT__ */ 


/*****************************************************************************
 * FUNCTION
 *  wgui_inline_edit_get_current_item_history
 * DESCRIPTION
 *  
 * PARAMETERS
 *  history_buffer      [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 *wgui_inline_edit_get_current_item_history(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (wgui_current_inline_item_type)
    {
        case INLINE_ITEM_TYPE_TEXT_EDIT:
            inline_text_edit_get_history((U8*) (history_buffer), MMI_current_input_type);
            break;
        case INLINE_ITEM_TYPE_FULL_SCREEN_EDIT:
            inline_full_screen_edit_get_history((U8*) (history_buffer), MMI_current_input_type);
            break;
        case INLINE_ITEM_TYPE_DATE:
            inline_date_edit_get_history((U8*) (history_buffer));
            break;
        case INLINE_ITEM_TYPE_TIME:
            inline_time_edit_get_history((U8*) (history_buffer));
            break;
        case INLINE_ITEM_TYPE_TIME_PERIOD:
            inline_time_period_edit_get_history((U8*) (history_buffer));
            break;
        case INLINE_ITEM_TYPE_IP4_EDIT:
            inline_IP4_edit_get_history((U8*) (history_buffer));
            break;
        case INLINE_ITEM_TYPE_DOW_SELECT:
            inline_DOW_select_get_history((U8*) (history_buffer));
            break;
        case INLINE_ITEM_TYPE_MULTI_LINE_EDIT:
            inline_multi_line_edit_get_history((U8*) (history_buffer), MMI_current_input_type);
            break;
    }
    wgui_current_history_buffer = history_buffer;
    return (history_buffer);
}

