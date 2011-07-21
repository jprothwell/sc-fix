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
/* **** Copy Right Note ********* 
 * Copyright Notice
 
 * All Rights Reserved.
 *  (It is illegal to remove this copyright notice from this software or any
 *  portion of it)
 *******************************************************************************
 * Filename:
 * ---------
 * wgui_draw_manager.c
 * Project:
 * --------
 * PDG2
 * Description:
 * ------------
 * Draws the category screens on the basis of the static coordinates specified
 * at Complile Time.
 * Author:
 * -------
 * -------
 *==============================================================================
 * HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

 /*  Include: MMI header file */
#include "mmi_features.h"

#ifdef __MMI_DRAW_MANAGER__
#include "frameworkstruct.h"
#include "custdatares.h"
#include "wgui_draw_manager.h"
#include "wgui_fixed_menus.h"
#include "wgui_status_icons.h"
#include "wgui_categories_mm.h"
#include "wgui_categories_inputs.h"
#include "wgui_ems_categories.h"
#include "wgui_categories_popup.h"
#include "wgui_categories_sublcd.h"
#include "wgui_categories_idlescreen.h"
#include "wgui_fixed_menus.h"
#include "wgui_softkeys.h"
#include "debuginitdef.h"
#include "gui_fixed_menuitems.h"
#include "lcd_sw.h"
#include "gdi_include.h"
#include "wgui_categories_util.h"
#include "wgui_calendar.h"
#include "wgui_tab_bars.h"
#include "wgui_virtual_keyboard.h"
#include "gui_setting.h"
#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif 
#if defined(__MMI_TOUCH_SCREEN__)
#include "editorpen.h"
#endif 
#ifdef __MMI_STOPWATCH__
#include "stopwatchdef.h"
#include "wgui_categories_stopwatch.h"
#include "stopwatchstruct.h"
#endif /* __MMI_STOPWATCH__ */ 
#ifdef __MMI_SCREEN_ROTATE__
#include "screenrotationgprot.h"
#endif 
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
#include "historygprot.h"
#endif 
#include "wallpaperdefs.h"
#include "poweronoffdisplaydefs.h"
#include "screensaverdefs.h"
#include "idleappdef.h"
#include "profilesenum.h"
#if defined(__SG_MMI_MEDIA_SPECIAL_MENU__)
extern void dm_setup_and_draw_meida_menu(S16 **UICtrlAccessPtr_p);
#endif
#include "quickaccessmenuitem_def.h"
extern U16 GetCurrScrnId(void);
#include "mainmenudef.h"
/***************************************************************************** 
* Local Variable
*****************************************************************************/

/**************************************************/
/* Currently, we keep two context for MainLCD and SubLCD */
/* In the future, we may have more than 2 LCDs                 */
/* More context data would be needed in the future              */
/**************************************************/

/* Start of Draw Manager MainLCD Context */

/* DM internal context */
static dm_cntx_struct g_dm_cntx;
static dm_scr_bg_cntx_struct g_dm_scr_bg_cntx;

/* DM components */

/* -Layers */
#ifndef __GDI_MEMORY_PROFILE_2__
static U16 new_layer_x = 0;
static U16 new_layer_y = 0;
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
/* Currently used layers in Draw Manager */
static GDI_HANDLE dm_layers[GDI_LAYER_TOTAL_LAYER_COUNT];

/* Total currently used layers in Draw Manager */
static U16 dm_layers_count = 0;
static GDI_HANDLE dm_wallpaper_layer = GDI_ERROR_HANDLE;

/* -Strings */
static dm_string_data_struct dm_string_array[DM_MAX_STRINGS];
static S32 dm_string_count = 0;

/* -Images */
static dm_image_data_struct dm_image_array[DM_MAX_IMAGES];
static S32 dm_image_count = 0;

/* -Background fillers */
static UI_filled_area *dm_back_fill_array[DM_MAX_FILL_AREAS];
static S32 dm_back_fill_count = 0;

/* -Rectangles */
static dm_rectangle_data_struct dm_rectangle_array[DM_MAX_RECTANGLES];
static S32 dm_rectangle_count = 0;

/* -Lines */
static color_t dm_line_array[DM_MAX_LINES];
static S32 dm_line_count = 0;

/* -Scrolling Text objects */
static scrolling_text dm_scroll_text[DM_MAX_SCROLL_TEXT];
static U8 dm_scroll_text_buf[DM_MAX_SCROLL_TEXT][MAX_SUBMENU_CHARACTERS];
static S32 dm_scroll_text_count = 0;
static scrolling_text *dm_current_scroll_text = NULL;

/* slide controls */
S32 dm_slide_control_count = 0;

/* buttons */
static icontext_button dm_button_array[DM_MAX_BUTTONS];
static S32 dm_button_count = 0;

#ifdef __MMI_DRAW_MANAGER__
static FuncPtr dm_button_func_ptrs[DM_MAX_BUTTONS][MAX_KEY_TYPE];
#endif 

/* percentage bar */
static dm_percentage_bar_struct dm_percentage_bar_array[DM_MAX_PERCENTAGE_BAR];
static S32 dm_percentage_bar_count = 0;

/* others */
static dm_previous_control_coordinates_struct prev_control_coordinates;
static dm_aligned_area_data_struct dm_aligned_area_data;
static FuncPtr dm_show_vkpad_callback = UI_dummy_function;

/* End of MainLCD context */

/* DM SubLCD context stat */
#ifdef __MMI_SUBLCD__

/* -Layers */
static GDI_HANDLE dm_layers_sub[GDI_LAYER_TOTAL_LAYER_COUNT];
static U16 dm_layers_count_sub = 0;

/* -Strings */
static dm_string_data_struct dm_string_array_sub[DM_MAX_SUBLCD_STRINGS];
static S32 dm_string_count_sub = 0;

/* -Images */
static dm_image_data_struct dm_image_array_sub[DM_MAX_SUBLCD_IMAGES];
static S32 dm_image_count_sub = 0;

#if (0) /* Currently Not used */
#endif /* (0) */ 

#endif /* __MMI_SUBLCD__ */ 
/* End of SubLCD context */

/***************************************************************************** 
* Global Variable
*****************************************************************************/
dm_data_struct g_dm_data;
dm_data_struct g_dm_sublcd_data;

void (*dm_backup_exit_category_function) (void);

#ifdef __MMI_SUBLCD__
void (*dm_sub_backup_exit_category_function) (void);
#endif 

void (*dm_category_controlled_area_callback) (dm_coordinates *coordinate);
void (*dm_category_controlled_area2_callback) (dm_coordinates *coordinate);

#ifdef __MMI_SUBLCD__
void (*dm_sub_category_controlled_area_callback) (dm_coordinates *coordinate);
#endif 
/***************************************************************************** 
* Global Function
*****************************************************************************/
S16 *dm_get_coordinates(S16 *UICtrlAccessPtr_p, dm_coordinates *dm_coordinate_info);

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/
#ifdef __MMI_STOPWATCH__
extern wtchSkinResStruct wtch_pos;
extern S32 stop_watch_x, stop_watch_y, stop_watch_width;
#endif /* __MMI_STOPWATCH__ */ 
extern S32 wgui_EMS_inputbox_information_bar_height;

extern U8 matrix_main_menu_highlight_bitmap_data[];
extern const U8 dm_base_control_set[];
extern const S16 dm_base_control_set_coordinates[];
extern icontext_button MMI_softkeys[];
extern BOOL r2lMMIFlag;

extern UI_image_ID_type  idle_screen_wallpaper_ID;     
extern S8 *idle_screen_wallpaper_name;
extern UI_image_ID_type  idle_screen_cache_id;    /* 073106 cache wallpaper */
extern S8 *idle_screen_cache_name;

/* Calendar Extern Variables */
extern S32 cat82_title1_x1;
extern S32 cat82_title1_y1;
extern S32 cat82_title1_x2;
extern S32 cat82_title1_y2;
extern S32 cat82_title2_x1;
extern S32 cat82_title2_y1;
extern S32 cat82_title2_x2;
extern S32 cat82_title2_y2;
extern S32 cat82_title3_x1;
extern S32 cat82_title3_y1;
extern S32 cat82_title3_x2;
extern S32 cat82_title3_y2;
extern pBOOL calendar_show_title2;
extern pBOOL calendar_show_title3;

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
extern S32 cat82_title1_month_x1;
extern S32 cat82_title1_month_y1;
extern S32 cat82_title1_month_x2;
extern S32 cat82_title1_month_y2;
extern pBOOL calendar_show_title_month;
extern BOOL lunar_calendar_present;
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 
extern U8 *Cat82_title1;
extern U8 *Cat82_title2;
extern U8 *Cat82_title3;
extern stFontAttribute wgui_dialer_box_f1;
extern UI_font_type calendar_title_font;

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
extern U8 *Cat82_title1_month;
extern UI_font_type calendar_title_month_font;
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

extern UI_filled_area wgui_pop_up_dialog_background;
extern gdi_handle wgui_layer_1;

extern multitap_input MMI_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_lowercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_bpmf_multitaps[MAX_MULTITAPS];

#if defined(__MMI_MULTITAP_SPANISH__)
extern multitap_input MMI_spanish_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_spanish_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_SPANISH__) */ 


    //qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
extern multitap_input MMI_tamil_multitaps[MAX_MULTITAPS];
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
extern multitap_input MMI_bengali_multitaps[MAX_MULTITAPS];
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
extern multitap_input MMI_punjabi_multitaps[MAX_MULTITAPS];
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
extern multitap_input MMI_telugu_multitaps[MAX_MULTITAPS];
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
extern multitap_input MMI_kannada_multitaps[MAX_MULTITAPS];
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
extern multitap_input MMI_malayalam_multitaps[MAX_MULTITAPS];
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
extern multitap_input MMI_oriya_multitaps[MAX_MULTITAPS];
#endif
//qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
extern multitap_input MMI_marathi_multitaps[MAX_MULTITAPS];
#endif
#if defined(__MMI_MULTITAP_URDU__)
extern multitap_input MMI_urdu_multitaps[MAX_MULTITAPS];
#endif

#if defined(__MMI_MULTITAP_FRENCH__)
extern multitap_input MMI_french_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_french_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_FRENCH__) */ 
#if defined(__MMI_MULTITAP_MYANMAR__)
extern multitap_input MMI_myanmar_multitaps[MAX_MULTITAPS];
#endif
#if defined(__MMI_MULTITAP_GERMAN__)
extern multitap_input MMI_german_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_german_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_GERMAN__) */ 
#if defined(__MMI_MULTITAP_ITALIAN__)
extern multitap_input MMI_italian_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_italian_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_ITALIAN__) */ 
 
#if defined(__MMI_MULTITAP_POLISH__)
extern multitap_input MMI_polish_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_polish_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_POLISH__) */ 
 
 
#if defined(__MMI_MULTITAP_CZECH__)
extern multitap_input MMI_czech_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_czech_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_CZECH__) */ 
 
 
#if defined(__MMI_MULTITAP_TURKISH__)
extern multitap_input MMI_turkish_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_turkish_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_TURKISH__) */ 
 
 

#if defined(__MMI_MULTITAP_VIETNAMESE__)
extern multitap_input MMI_vietnamese_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_vietnamese_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_VIETNAMESE__) */ 

/*+ zhouqin modify for indonesian full keypad 20110419*/
#if defined(__MMI_MULTITAP_INDONESIAN__) && !defined(__MMI_FULL_KEYPAD__)
/*- zhouqin modify for indonesian full keypad 20110419*/
extern multitap_input MMI_indonesian_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_indonesian_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_INDONESIAN__) */ 

#if defined(__MMI_MULTITAP_DANISH__)
extern multitap_input MMI_danish_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_danish_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_DANISH__) */ 

#if defined(__MMI_MULTITAP_PORTUGUESE__)&& !defined(__MMI_FULL_KEYPAD__)
extern multitap_input MMI_portuguese_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_portuguese_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_PORTUGUESE__) */ 
 
#if defined(__MMI_MULTITAP_FINNISH__)
extern multitap_input MMI_finnish_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_finnish_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_FINNISH__) */ 
 
 
#if defined(__MMI_MULTITAP_SLOVAK__)
extern multitap_input MMI_slovak_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_slovak_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_SLOVAK__) */ 
/* START DUTCH_T9 20051021 */
#if defined(__MMI_MULTITAP_DUTCH__)
extern multitap_input MMI_dutch_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_dutch_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_DUTCH__) */ 
//END DUTCH_T9 20051021
 
#if defined(__MMI_MULTITAP_BULGARIAN__)
extern multitap_input MMI_bulgarian_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_bulgarian_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_BULGARIAN__) */ 
#if defined(__MMI_MULTITAP_ARABIC__)
extern multitap_input MMI_arabic_multitaps[MAX_MULTITAPS];
#endif 
/* Norwegian start */
#if defined(__MMI_MULTITAP_NORWEGIAN__)
extern multitap_input MMI_norwegian_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_norwegian_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_NORWEGIAN__) */ 
//Norwegian end
 
 
#if defined(__MMI_MULTITAP_SWEDISH__)
extern multitap_input MMI_swedish_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_swedish_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_SWEDISH__) */ 
 
 
#if defined(__MMI_MULTITAP_CROATIAN__)
extern multitap_input MMI_croatian_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_croatian_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_CROATIAN__) */ 
#if defined(__MMI_MULTITAP_ROMANIAN__)
extern multitap_input MMI_romanian_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_romanian_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_ROMANIAN__) */ 
#if defined(__MMI_MULTITAP_SLOVENIAN__)
extern multitap_input MMI_slovenian_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_slovenian_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_SLOVENIAN__) */ 
 
 
#if defined(__MMI_MULTITAP_GREEK__)
extern multitap_input MMI_greek_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_greek_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_GREEK__) */ 
#if defined(__MMI_MULTITAP_HUNGARIAN__)
extern multitap_input MMI_hungarian_uppercase_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_hungarian_lowercase_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_MULTITAP_HUNGARIAN__) */ 
 
#if defined(__MMI_WCSS_INPUT_FORMAT_SUPPORT__)
extern multitap_input MMI_uppercase_numberless_multitaps[MAX_MULTITAPS];
extern multitap_input MMI_lowercase_numberless_multitaps[MAX_MULTITAPS];
#endif /* defined(__MMI_WCSS_INPUT_FORMAT_SUPPORT__) */ 

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
extern void (*redraw_current_inline_item) (void);
extern S32 dm_get_coordinate_sets_count(void);
extern S32 dm_get_control_sets_count(void);
extern U8 *dm_get_base_control_set(void);
extern S16 *dm_get_base_coordinate_set(void);
extern U8 *dm_get_base_control_set2(void);
extern S16 *dm_get_base_coordinate_set2(void);

extern void set_calendar_control_background_limits(S32 y1, S32 y2);
extern void set_button_bar_height(S32 height);
extern void wgui_reset_calendar_params(S32 x, S32 y, S32 n_columns, S32 n_rows, S32 cell_width, S32 cell_height);
extern void wgui_show_calendar(void);
extern void ChangeCategory82Title1(U8 *title);
extern void ChangeCategory82Title2(U8 *title);
extern void ChangeCategory82Title3(U8 *title);
extern void category151_keyboard_handler(S32 vkey_code, S32 key_state);
extern void UI_cross_hatch_fill_rectangle(S32 x1, S32 y1, S32 x2, S32 y2, color_t c);
extern MMI_BOOL get_singleline_inputbox_display_status(void);
extern MMI_BOOL is_multiline_inputbox_in_view_mode(void);
extern MMI_BOOL is_EMS_inputbox_in_view_mode(void);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
extern void ChangeCategory82Title1Month(U8 *title);
#endif 
#ifdef __MMI_STOPWATCH__
extern void get_stop_watch_history(U16 history_ID, U8 *history_buffer);
#endif 


/*****************************************************************************
 * FUNCTION
 *  dm_init
 * DESCRIPTION
 *  initialize variables used in draw manager.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 idx = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (idx = 0; idx < GDI_LAYER_TOTAL_LAYER_COUNT; idx++)
    {
        dm_layers[idx] = GDI_ERROR_HANDLE;
    }

    /* Draw Manager context */
    g_dm_cntx.redraw_screen_main = NORMAL_EXIT;
    g_dm_cntx.redraw_scr_bg_flag = 1;
    g_dm_cntx.lock_redraw_main = 0;
    g_dm_cntx.exit_full_screen_on_exit = 0;
    g_dm_cntx.is_context_reset = 1;
#ifdef __MMI_SUBLCD__
    g_dm_cntx.redraw_screen_sub = NORMAL_EXIT;
    g_dm_cntx.lock_redraw_sub = 0;
#endif /* __MMI_SUBLCD__ */ 

    /* Draw Manager Screen Background Layer Context */
    g_dm_scr_bg_cntx.layer_handle = GDI_ERROR_HANDLE;
    /* g_dm_scr_bg_cntx.animation_handle=GDI_ERROR_HANDLE; */
    g_dm_scr_bg_cntx.image_id = 0;
    g_dm_scr_bg_cntx.image_name = NULL;
    g_dm_scr_bg_cntx.offset_x = -1;
    g_dm_scr_bg_cntx.offset_y = -1;
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_NOKIA_STYLE_N800__)||(defined __MMI_MAINLCD_220X176__) || defined(__MMI_MAINLCD_160X128__) || defined(__MMI_MAINLCD_320X240__) || defined(__MMI_MAINLCD_240X320__) 
    g_dm_scr_bg_cntx.layer_opacity = 255;
#else
    g_dm_scr_bg_cntx.layer_opacity = 150;

#endif
    g_dm_scr_bg_cntx.flags = 0;//082906 scr_bg

    /* Reset all images */
    for (idx = 0; idx < dm_image_count; idx++)
    {
        dm_image_array[idx].back_fill = NULL;
        dm_image_array[idx].image = NULL;
        dm_image_array[idx].name = NULL;
        if (dm_image_array[idx].image_handle != GDI_ERROR_HANDLE)
        {
            gdi_image_stop_animation(dm_image_array[idx].image_handle);
            dm_image_array[idx].image_handle = GDI_ERROR_HANDLE;
        }
    }
    dm_image_count = 0;

    dm_string_count = 0;
    dm_back_fill_count = 0;
    dm_rectangle_count = 0;
    dm_line_count = 0;
    dm_slide_control_count = 0;
    dm_scroll_text_count = 0;
    dm_current_scroll_text = NULL;
    dm_button_count = 0;

#ifdef __MMI_SUBLCD__
    /* Reset all images */
    for (idx = 0; idx < dm_image_count_sub; idx++)
    {
        dm_image_array_sub[idx].back_fill = NULL;
        dm_image_array_sub[idx].image = NULL;
        dm_image_array_sub[idx].name = NULL;
        if (dm_image_array_sub[idx].image_handle != GDI_ERROR_HANDLE)
        {
            gdi_image_stop_animation(dm_image_array_sub[idx].image_handle);
            dm_image_array_sub[idx].image_handle = GDI_ERROR_HANDLE;
        }
    }
    dm_image_count_sub = 0;
    dm_string_count_sub = 0;
#endif /* __MMI_SUBLCD__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  dm_reset_context
 * DESCRIPTION
 *  reset variables used in draw manager.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_context(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	
    if (g_dm_cntx.is_context_reset == MMI_FALSE)
    {
        U8 idx = 0;
        /* Reset screen background layer */
        dm_set_scr_bg_redraw(MMI_TRUE);
#if defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__)
#else
        g_dm_scr_bg_cntx.layer_handle = GDI_ERROR_HANDLE;
#endif
        /* Reset Layer information */
        /* MainLCD */
        if (!(g_dm_data.S32flags & DM_SUB_LCD_CONTEXT))
        {
            for (idx = 0; idx < dm_layers_count; idx++)
            {
                if (dm_layers[idx] != GDI_LAYER_MAIN_BASE_LAYER_HANDLE && dm_layers[idx] != wgui_layer_1
                    && dm_layers[idx] != GDI_ERROR_HANDLE && dm_layers[idx] != GDI_LAYER_EMPTY_HANDLE
                    #if defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__)
                    &&dm_layers[idx] != g_dm_scr_bg_cntx.layer_handle
                    #endif
                    )
                {
                    gdi_layer_free(dm_layers[idx]);
                }
                dm_layers[idx] = GDI_ERROR_HANDLE;
            }
            dm_layers_count = 0;
        }
	  gdi_layer_set_blt_layer(dm_layers[0],dm_layers[1],dm_layers[2],dm_layers[3]);
        dm_wallpaper_layer = GDI_ERROR_HANDLE;

        /* can be removed */
        gdi_layer_push_and_set_active(GDI_LAYER_MAIN_BASE_LAYER_HANDLE);
        /* gdi_layer_set_source_key(FALSE,GDI_COLOR_TRANSPARENT); */
        gdi_layer_pop_and_restore_active();

        if (g_dm_cntx.redraw_screen_main == REDRAW_2_SHOW)
        {
            return;
        }

        /* Reset all images */
        for (idx = 0; idx < dm_image_count; idx++)
        {
            dm_image_array[idx].back_fill = NULL;
            dm_image_array[idx].image = NULL;
            dm_image_array[idx].name = NULL;
            if (dm_image_array[idx].image_handle != GDI_ERROR_HANDLE)
            {
                gdi_image_stop_animation(dm_image_array[idx].image_handle);
                dm_image_array[idx].image_handle = GDI_ERROR_HANDLE;
            }
        }

        for (idx = 0; idx < dm_scroll_text_count; idx++)
        {
            gui_scrolling_text_stop(&(dm_scroll_text[idx]));
        }
        dm_image_count = 0;

        dm_string_count = 0;
        dm_back_fill_count = 0;
        dm_rectangle_count = 0;
        dm_line_count = 0;
        dm_slide_control_count = 0;
        dm_scroll_text_count = 0;
        dm_current_scroll_text = NULL;
        dm_button_count = 0;
        dm_percentage_bar_count = 0;

        /* Reset default pen handlers */
    #ifdef __MMI_TOUCH_SCREEN__
        wgui_reset_category_screen_control_pen_handler();
        wgui_reset_image_control_pen_handler();
    #endif /* __MMI_TOUCH_SCREEN__ */ 

        dm_reset_category_controlled_callback();
        dm_reset_button_functions();

        /* Can be removed -start */
        dm_reset_title_coordinates();
        dm_reset_fixed_list_coordinates();
        dm_reset_inline_fixed_list_coordinates();
        dm_reset_lsk_coordinates();
        dm_reset_rsk_coordinates();
        dm_reset_button_bar_coordinates();
        /* Can be removed -end */

        dm_show_vkpad_callback = NULL;

    #ifdef __MMI_SCREEN_ROTATE__
        /* Reset the background image set by rotated screen */
        if (mmi_frm_is_screen_width_height_swapped())
        {
            g_dm_scr_bg_cntx.image_id = 0;
            g_dm_scr_bg_cntx.image_name = NULL;
            //g_dm_scr_bg_cntx.flags = 0;//082906 scr_bg
        }
    #endif /* __MMI_SCREEN_ROTATE__ */ 

        g_dm_data.S32CatId = 0;
        g_dm_data.S32flags = 0;
        g_dm_data.S32ScrId = 0;

         
        ExitCategoryFunction = MMI_dummy_function;
        RedrawCategoryFunction = MMI_dummy_function;
        GetCategoryHistory = dummy_get_history;
        GetCategoryHistorySize = dummy_get_history_size;
         
        g_dm_cntx.is_context_reset = MMI_TRUE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  dm_reset_context_ext
 * DESCRIPTION
 *  reset variables used in draw manager.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_context_ext(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}

#ifdef __MMI_SUBLCD__


/*****************************************************************************
 * FUNCTION
 *  dm_reset_sublcd_context
 * DESCRIPTION
 *  reset variables used in draw manager for sublcd.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_sublcd_context(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 idx = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (idx = 0; idx < dm_layers_count_sub; idx++)
    {
        if (dm_layers_sub[idx] != GDI_LAYER_SUB_BASE_LAYER_HANDLE)
        {
            gdi_layer_free(dm_layers_sub[idx]);
        }
        dm_layers_sub[idx] = GDI_ERROR_HANDLE;
    }
    dm_layers_count_sub = 0;

    /* Reset all images */
    for (idx = 0; idx < dm_image_count_sub; idx++)
    {
        dm_image_array_sub[idx].back_fill = NULL;
        dm_image_array_sub[idx].image = NULL;
        dm_image_array_sub[idx].name = NULL;
        if (dm_image_array_sub[idx].image_handle != GDI_ERROR_HANDLE)
        {
            gdi_image_stop_animation(dm_image_array_sub[idx].image_handle);
            dm_image_array_sub[idx].image_handle = GDI_ERROR_HANDLE;
        }
    }
    dm_image_count_sub = 0;
    dm_string_count_sub = 0;
    dm_reset_sub_category_controlled_callback();
}
#endif /* __MMI_SUBLCD__ */ 


/*****************************************************************************
 * FUNCTION
 *  dm_get_image_count
 * DESCRIPTION
 *  Used to get currently used image controls
 * PARAMETERS
 *  void
 * RETURNS
 *  U16      number of image countrol used currently
 *****************************************************************************/
U16 dm_get_image_count(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return dm_image_count;
}


/*****************************************************************************
 * FUNCTION
 *  dm_get_button_count
 * DESCRIPTION
 *  Used to get currently used button controls
 * PARAMETERS
 *  void
 * RETURNS
 *  U16      number of button countrol used currently
 *****************************************************************************/
U16 dm_get_button_count(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return dm_button_count;
}


/*****************************************************************************
 * FUNCTION
 *  dm_get_button
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
icontext_button *dm_get_button(U16 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(index < DM_MAX_BUTTONS);

    return &dm_button_array[index];
}

 


/*****************************************************************************
 * FUNCTION
 *  dm_get_font
 * DESCRIPTION
 *  Used to return the font poointer depending on the font enum value read from
 *  coordinate set
 * PARAMETERS
 *  font        [IN]        Font enum value
 * RETURNS
 *  returns font pointer
 *****************************************************************************/
UI_font_type dm_get_font(S16 font)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (font)
    {
        case SMALL_FONT:
            return (&MMI_small_font);
            break;
        case MEDIUM_FONT:
            return (&MMI_medium_font);
            break;
        case LARGE_FONT:
            return (&MMI_large_font);
        case SUBLCD_FONT:
            return (&MMI_sublcd_font);
        case DIALER_FONT:
            return (&wgui_dialer_box_f1);
        default:
            return (&MMI_default_font);
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  setup_previous_control
 * DESCRIPTION
 *  Set ups the coordinates for previous control.
 * PARAMETERS
 *  dm_coordinate_info      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void setup_previous_control(dm_coordinates *dm_coordinate_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    prev_control_coordinates.coordinate.S16X = dm_coordinate_info->S16X;
    prev_control_coordinates.coordinate.S16Y = dm_coordinate_info->S16Y;
    prev_control_coordinates.coordinate.S16Height = dm_coordinate_info->S16Height;
    prev_control_coordinates.coordinate.S16Width = dm_coordinate_info->S16Width;
    prev_control_coordinates.S16X2 =
        prev_control_coordinates.coordinate.S16X + prev_control_coordinates.coordinate.S16Width - 1;
    prev_control_coordinates.S16Y2 =
        prev_control_coordinates.coordinate.S16Y + prev_control_coordinates.coordinate.S16Height - 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_exit_category_function
 * DESCRIPTION
 *  Common exit function for draw manager.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_exit_category_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (g_dm_cntx.redraw_screen_main)
    {
            /* normal case */
        case NORMAL_REDRAW:
        case REDRAW_2_REDRAW:
            if (dm_backup_exit_category_function)
            {
                dm_backup_exit_category_function();
            }
            g_dm_cntx.redraw_screen_main = NORMAL_EXIT;
            break;

        case REDRAW_2_SHOW:
            break;

        default:
            MMI_ASSERT(0);
    }

#ifdef GDI_USING_LAYER_ROTATE
     
    //if ( gdi_layer_get_base_layer_rotation() == GDI_LCD_LAYER_ROTATE_0)
    if (gdi_lcd_get_rotate() == GDI_LCD_LAYER_ROTATE_0)
#endif /* GDI_USING_LAYER_ROTATE */ 
    {
        if (dm_layers[0] != GDI_ERROR_HANDLE || dm_layers[1] != GDI_ERROR_HANDLE || dm_layers[2] != GDI_ERROR_HANDLE ||
            dm_layers[3] != GDI_ERROR_HANDLE)
        {
            gdi_layer_flatten_to_base(dm_layers[0], dm_layers[1], dm_layers[2], dm_layers[3]);
        }
    }
    dm_reset_context();

    if (g_dm_cntx.exit_full_screen_on_exit)
    {
        g_dm_cntx.exit_full_screen_on_exit = MMI_FALSE;
        leave_full_screen();
    }

     
    if (wgui_is_wallpaper_on_bottom())
    {
        gOnFullScreen = MMI_LEAVE_BG_ON_BOTTOM_SCREEN;
        wgui_reset_wallpaper_on_bottom();    
    }
     
}   /* end of dm_exit_category_function */

#ifdef __MMI_SUBLCD__


/*****************************************************************************
 * FUNCTION
 *  dm_sub_exit_category_function
 * DESCRIPTION
 *  Common exit function for draw manager.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_sub_exit_category_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_dm_sublcd_data.S32flags & DM_SUB_LCD_CONTEXT)
    {
        switch (g_dm_cntx.redraw_screen_sub)
        {
                /* normal case */
            case NORMAL_REDRAW:
            case REDRAW_2_REDRAW:
                if (dm_sub_backup_exit_category_function)
                {
                    dm_sub_backup_exit_category_function();
                }
                g_dm_cntx.redraw_screen_sub = NORMAL_EXIT;
                break;

            case REDRAW_2_SHOW:
                break;

            default:
                MMI_ASSERT(0);
        }
    }
    dm_reset_sublcd_context();
    SUBLCD_ExitCategoryFunction = MMI_dummy_function;
    SUBLCD_RedrawCategoryFunction = MMI_dummy_function;
    SUBLCD_GetCategoryHistory = dummy_get_history;
    SUBLCD_GetCategoryHistorySize = dummy_get_history_size;

}

#endif /* __MMI_SUBLCD__ */ 


/*****************************************************************************
 * FUNCTION
 *  dm_backup_current_category_exit_function
 * DESCRIPTION
 *  Save current exit function and set ExitCategoryFunction to dm_exit_category_function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_backup_current_category_exit_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_backup_exit_category_function = ExitCategoryFunction;
    ExitCategoryFunction = dm_exit_category_function;
}   /* end of dm_backup_current_category_exit_function */

#ifdef __MMI_SUBLCD__


/*****************************************************************************
 * FUNCTION
 *  dm_sub_backup_current_category_exit_function
 * DESCRIPTION
 *  Save current exit function and set ExitCategoryFunction to dm_exit_category_function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_sub_backup_current_category_exit_function(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_sub_backup_exit_category_function = SUBLCD_ExitCategoryFunction;
    SUBLCD_ExitCategoryFunction = dm_sub_exit_category_function;
}   /* end of dm_backup_current_category_exit_function */
#endif /* __MMI_SUBLCD__ */ 


/*****************************************************************************
 * FUNCTION
 *  dm_setup_category_functions
 * DESCRIPTION
 *  This function sets up the common-used functions for the Draw Manager
 * PARAMETERS
 *  redraw_function                 [IN]        
 *  get_history_function            [IN]        
 *  get_history_size_function       [IN]        
 *  dm_data(?)                      [IN]        Which contains the Draw Manager Information
 * RETURNS
 *  void
 *****************************************************************************/
void dm_setup_category_functions(
        FuncPtr redraw_function,
        U8 *(*get_history_function) (U8 *buffer),
        S32(*get_history_size_function) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategoryFunction = redraw_function;
    GetCategoryHistory = get_history_function;
    GetCategoryHistorySize = get_history_size_function;
}

#ifdef __MMI_SUBLCD__


/*****************************************************************************
 * FUNCTION
 *  dm_sublcd_setup_data
 * DESCRIPTION
 *  This function sets up the information for the Sublcd Draw Manager
 * PARAMETERS
 *  dm_data     [IN]        Which contains the Draw Manager Information
 * RETURNS
 *  void
 *****************************************************************************/
void dm_sublcd_setup_data(dm_data_struct *dm_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_dm_sublcd_data.S32CatId = dm_data->S32CatId;
    g_dm_sublcd_data.S32ScrId = dm_data->S32ScrId;
    g_dm_sublcd_data.S32flags = dm_data->S32flags;
    dm_sub_backup_current_category_exit_function();
    if (g_dm_sublcd_data.S32CatId == 0 && g_dm_sublcd_data.S32ScrId == 0 && g_dm_sublcd_data.S32flags == 0)
    {
        return;
    }

    if (g_dm_sublcd_data.S32flags & DM_SUB_LCD_CONTEXT)
    {
        /* Normal procedure */
        if (g_dm_cntx.redraw_screen_sub == NORMAL_EXIT)
        {
            g_dm_cntx.redraw_screen_sub = NORMAL_SHOW;
        }

        /* from redraw screen */
        else if (g_dm_cntx.redraw_screen_sub == NORMAL_REDRAW)
        {
            g_dm_cntx.redraw_screen_sub = NORMAL_REDRAW;
            /* MMI_ASSERT (0); */
        }

        /* from show screen */
        //              else if ( g_dm_cntx.redraw_screen_sub == NORMAL_SHOW )
        //                      MMI_ASSERT (0);
    }
}   /* end of dm_setup_data */


/*****************************************************************************
 * FUNCTION
 *  dm_sublcd_redraw_category_screen
 * DESCRIPTION
 *  This function redraws the Sublcd.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_sublcd_redraw_category_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct g_dm_data_temp_struct;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_set_sub_LCD_graphics_context();
    g_dm_data_temp_struct = g_dm_data;
    g_dm_data = g_dm_sublcd_data;
    g_dm_data.S32flags |= DM_SUB_LCD_CONTEXT;
    dm_redraw_category_screen();
    g_dm_data = g_dm_data_temp_struct;
    UI_set_main_LCD_graphics_context();
}   /* end of dm_setup_data */
#endif /* __MMI_SUBLCD__ */ 


/*****************************************************************************
 * FUNCTION
 *  dm_is_activated
 * DESCRIPTION
 *  Check whether the current
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL dm_is_activated(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_dm_data.S32CatId == 0)
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
 *  dm_setup_data
 * DESCRIPTION
 *  This function sets up the information for the Draw Manager
 * PARAMETERS
 *  dm_data     [IN]        Which contains the Draw Manager Information
 * RETURNS
 *  void
 *****************************************************************************/
void dm_setup_data(dm_data_struct *dm_data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_dm_data.S32CatId = dm_data->S32CatId;
    g_dm_data.S32ScrId = dm_data->S32ScrId;
    g_dm_data.S32flags = dm_data->S32flags;

    dm_backup_current_category_exit_function();

    if (g_dm_data.S32CatId == 0 && g_dm_data.S32ScrId == 0 && g_dm_data.S32flags == 0)
    {
        return;
    }

    /* Normal procedure */
    if (g_dm_cntx.redraw_screen_main == NORMAL_EXIT)
    {
        g_dm_cntx.redraw_screen_main = NORMAL_SHOW;
    }

    /* from redraw screen */
    else if (g_dm_cntx.redraw_screen_main == NORMAL_REDRAW)
    {
        g_dm_cntx.redraw_screen_main = REDRAW_2_SHOW;
        dm_reset_context();
        g_dm_cntx.redraw_screen_main = NORMAL_SHOW;
        /* MMI_ASSERT (0); */
    }

    /* from show screen */
    else if (g_dm_cntx.redraw_screen_main == NORMAL_SHOW)
    {
        ;
    }
    /* MMI_ASSERT (0); */

#ifdef __MMI_SCREEN_ROTATE__

    if (mmi_frm_is_screen_width_height_swapped())
    {
        /* Close status icon (but not entry_full_screen()) */
        close_title_status_icon();
        g_dm_cntx.exit_full_screen_on_exit = MMI_TRUE;
        /* Set screen background */
        g_dm_data.S32flags |= DM_CLEAR_SCREEN_BACKGROUND;

    #ifdef __MMI_WALLPAPER_ON_BOTTOM__
        /* Keep opacity unchanged */
        dm_set_scr_bg_image(IMG_SCREEN_ROTATE_BACKGROUND, NULL, -1, -1, g_dm_scr_bg_cntx.layer_opacity);
    #else /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
        dm_set_scr_bg_image(IMG_SCREEN_ROTATE_BACKGROUND, NULL, -1, -1, 255);
    #endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
    }
else
#endif /* __MMI_SCREEN_ROTATE__ */ 
	if(g_dm_data.S32CatId != MMI_CATEGORY129_ID // added by wangmg: PowerOnOffSysView image always wallpaper
		 && g_dm_data.S32CatId != MMI_CATEGORY105_ID) // Volume Level screen never reset scr bg
	{	//reset scr bg for V mode
			g_dm_scr_bg_cntx.image_id = 0;
			g_dm_scr_bg_cntx.image_name = NULL;
    }

}   /* end of dm_setup_data */


/*****************************************************************************
 * FUNCTION
 *  dm_search_coordinate_set
 * DESCRIPTION
 *  This function is used to search (binary search) the coordinate set corresponding to the passed Screen Id.
 * PARAMETERS
 *  ScrId           [IN]        
 *  S32ScrId(?)     [IN]        Screen Id corresponding to which coordinate set is to be obtained.
 * RETURNS
 *  void
 *****************************************************************************/
S16 *dm_search_coordinate_set(S32 ScrId)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 StartIndex = 0;
    S32 EndIndex = dm_get_coordinate_sets_count();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (EndIndex >= StartIndex)
    {
        S32 MiddleIndex = (EndIndex + StartIndex) >> 1;

        if (g_screenid_coordinate_sets_map[MiddleIndex].screen_id == ScrId)
        {
            return g_screenid_coordinate_sets_map[MiddleIndex].coordinate_set_p;
        }
        else if (g_screenid_coordinate_sets_map[MiddleIndex].screen_id > ScrId)
        {
            EndIndex = MiddleIndex - 1;
        }
        else
        {
            StartIndex = MiddleIndex + 1;
        }
    }
    return NULL;
}   /* end of dm_search_coordinate_set */


/*****************************************************************************
 * FUNCTION
 *  dm_search_control_set
 * DESCRIPTION
 *  get control set address by category id
 * PARAMETERS
 *  category_id                     [IN]        Id of category screen
 *  default_coordinate_set_p        [IN]        
 * RETURNS
 *  U8 address of corresponding control set
 *****************************************************************************/
U8 *dm_search_control_set(S32 category_id, S16 **default_coordinate_set_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 min = 0, max = 0, mid = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* MMI_ASSERT(category_id); */
    if (category_id == 0)
    {
        return NULL;
    }

    max = dm_get_control_sets_count();
     
    min = 0;
    *default_coordinate_set_p = NULL;
    while (min <= max)
    {
        mid = (max + min) >> 1;
        if (g_categories_controls_map[mid].category_id == (U16) category_id)
        {
        #ifdef __MMI_SCREEN_ROTATE__
            MMI_DBG_ASSERT(!mmi_frm_is_screen_width_height_swapped() ||
                           g_categories_controls_map[mid].rotated_coordinate_set_p);
            if (mmi_frm_is_screen_width_height_swapped() && g_categories_controls_map[mid].rotated_coordinate_set_p)
            {
                *default_coordinate_set_p = g_categories_controls_map[mid].rotated_coordinate_set_p;
            }
            else
            {
                *default_coordinate_set_p = g_categories_controls_map[mid].default_coordinate_set_p;
            }
        #else /* __MMI_SCREEN_ROTATE__ */ 
            *default_coordinate_set_p = g_categories_controls_map[mid].default_coordinate_set_p;
        #endif /* __MMI_SCREEN_ROTATE__ */ 
            return g_categories_controls_map[mid].control_set_p;
        }
        else if (g_categories_controls_map[mid].category_id > (U16) category_id)
        {
            max = mid - 1;
        }
        else
        {
            min = mid + 1;
        }
    }
    MMI_ASSERT(0);
    return NULL;
}   /* end of dm_search_control_set */


/*****************************************************************************
 * FUNCTION
 *  dm_setup_base_layer
 * DESCRIPTION
 *  This function is used setup the base layer
 * PARAMETERS
 *  void
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_base_layer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    GDI_HANDLE act_lcd_handle;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_lcd_get_active(&act_lcd_handle);
    if (act_lcd_handle == GDI_LCD_SUB_LCD_HANDLE && !(g_dm_data.S32flags & DM_SUB_LCD_CONTEXT))
    {
        MMI_ASSERT(0);
    }

#ifdef __MMI_SUBLCD__
    if (g_dm_data.S32flags & DM_SUB_LCD_CONTEXT)
    {
        gdi_layer_get_base_handle(&dm_layers_sub[dm_layers_count_sub++]);
    }
    else
#endif /* __MMI_SUBLCD__ */ 
        gdi_layer_get_base_handle(&dm_layers[dm_layers_count++]);

    gdi_layer_restore_base_active();
    /* not to set source key when sublcd is b/w */
#if (defined __MMI_SUBLCD__) && (!defined __MMI_SUBLCD_COLOR__)
    if ( act_lcd_handle == GDI_LCD_SUB_LCD_HANDLE )
    {
        gdi_layer_set_source_key(FALSE,0);
    }
    else
    {
        gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
    }
#else
	gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
#endif

#ifndef __GDI_MEMORY_PROFILE_2__
    new_layer_x = 0;
    new_layer_y = 0;
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_new_layer
 * DESCRIPTION
 *  This function is used setup a new layer
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_new_layer(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_layer_info;
    gdi_result ret;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_layer_multi_layer_enable();
#endif 

    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_layer_info);
    /* need to take care in REDRAW_2_REDRAW */
    if (g_dm_cntx.redraw_screen_main == REDRAW_2_REDRAW)
    {
        return 1;
    }

    if (dm_layer_info.S16X == -1 && dm_layer_info.S16Y == -1 && dm_layer_info.S16Width == -1 &&
        dm_layer_info.S16Height == -1)
    {
        return 1;
    }

#ifdef __GDI_MEMORY_PROFILE_2__ /* Multi-layer enable */

#ifdef __MMI_SUBLCD__

    if (g_dm_data.S32flags & DM_SUB_LCD_CONTEXT)
    {
        ret = gdi_layer_create(
                dm_layer_info.S16X,
                dm_layer_info.S16Y,
                dm_layer_info.S16Width,
                dm_layer_info.S16Height,
                &dm_layers_sub[dm_layers_count_sub]);
        MMI_ASSERT(ret >= 0);
        gdi_layer_set_active(dm_layers_sub[dm_layers_count_sub]);
        dm_layers_count_sub++;
    }
    else
#endif /* __MMI_SUBLCD__ */ 

    {
        ret = gdi_layer_create(
                dm_layer_info.S16X,
                dm_layer_info.S16Y,
                dm_layer_info.S16Width,
                dm_layer_info.S16Height,
                &dm_layers[dm_layers_count]);
        MMI_ASSERT(ret >= 0);
        gdi_layer_set_active(dm_layers[dm_layers_count]);
        dm_layers_count++;
    }
    /* 072706 source key Start */
    gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT); /* source key color could be customized later */
    gdi_layer_clear(GDI_COLOR_TRANSPARENT);
    /* 072706 source key End */
#else /* __GDI_MEMORY_PROFILE_2__ */ 
    new_layer_x = (U16) dm_layer_info.S16X;
    new_layer_y = (U16) dm_layer_info.S16Y;
#endif /* __GDI_MEMORY_PROFILE_2__ */ 

    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_new_layer
 * DESCRIPTION
 *  This function is used setup a new layer
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
 #ifdef __MMI_ANIMATION_DISPALY__
extern void draw_fish_element();
extern void draw_butterfly_element();
 GDI_HANDLE dm_animation_display_handle;
 void dm_animation_display_timer_func(viod)
{


	  
	gdi_layer_push_and_set_active(dm_animation_display_handle);
	gui_lock_double_buffer();
	gdi_image_layer_copy(0, 0,UI_device_width-1, UI_device_height-1,dm_animation_display_handle,dm_get_scr_bg_layer());
	draw_fish_element();
	draw_butterfly_element();
	gui_unlock_double_buffer();
	gdi_layer_pop_and_restore_active();
	gdi_layer_blt(
            dm_layers[0],
            dm_layers[1],
            dm_layers[2],
            dm_layers[3],
            0,
            0,
            UI_device_width-1, UI_device_height-1);
	coolsand_UI_start_timer(1,dm_animation_display_timer_func);
 }
extern void init_draw_fish_element();
extern void init_draw_butterfly_element();
S32 dm_setup_new_layer_for_animation_display(S16 **UICtrlAccessPtr_p)
{

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_layer_info;
    gdi_result ret;
    int i=0;
    int layer_idx = 0;
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_layer_multi_layer_enable();
#endif 

    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_layer_info);
    /* need to take care in REDRAW_2_REDRAW */
    if (g_dm_cntx.redraw_screen_main == REDRAW_2_REDRAW)
    {
        return 1;
    }

    if (dm_layer_info.S16X == -1 && dm_layer_info.S16Y == -1 && dm_layer_info.S16Width == -1 &&
        dm_layer_info.S16Height == -1)
    {
        return 1;
    }

    {
        ret = gdi_layer_create(
                dm_layer_info.S16X,
                dm_layer_info.S16Y,
                dm_layer_info.S16Width,
                dm_layer_info.S16Height,
                &dm_layers[dm_layers_count]);
        MMI_ASSERT(ret >= 0);
	 dm_animation_display_handle = dm_layers[dm_layers_count];
        gdi_layer_set_active(dm_layers[dm_layers_count]);

     for (layer_idx = dm_layers_count -1; layer_idx < dm_layers_count; layer_idx++)
        {
            dm_layers[layer_idx + 1] = dm_layers[layer_idx];
        }
        dm_layers[0] = dm_get_scr_bg_layer();
	 dm_layers[1] = dm_animation_display_handle;
        dm_layers_count++;
    }

    gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT); /* source key color could be customized later */
    gdi_layer_clear(GDI_COLOR_TRANSPARENT);
    init_draw_fish_element();
    init_draw_butterfly_element();
    dm_animation_display_timer_func();

    return 1;
}
#endif
#if((ENABLE_CIRCULAR_3D_MENU) || (ENABLE_ROTATE_MENU))				

/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_circular_menu
 * DESCRIPTION
 *  This function is used setup the circular menu based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_circular_menu(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_3D_circular_menu_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_3D_circular_menu_info);
    if (dm_3D_circular_menu_info.S16X == -1 || dm_3D_circular_menu_info.S16Y == -1 ||
        dm_3D_circular_menu_info.S16Width == -1 || dm_3D_circular_menu_info.S16Height == -1)
    {
        return 0;
    }
    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        move_circular_3d_menu(dm_3D_circular_menu_info.S16X, dm_3D_circular_menu_info.S16Y);
        resize_circular_3d_menu(dm_3D_circular_menu_info.S16Width, dm_3D_circular_menu_info.S16Height);
    }
    show_circular_3d_menu();
    return 1;
}
#endif

/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_fixed_list
 * DESCRIPTION
 *  This function is used setup the fixed list based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_fixed_list(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_fixed_list_info;
    MMI_BOOL resized = MMI_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_fixed_list_info);

    if (dm_fixed_list_info.S16X == -1 || dm_fixed_list_info.S16Y == -1 ||
        dm_fixed_list_info.S16Width == -1 || dm_fixed_list_info.S16Height == -1)
    {
        return 0;
    }

    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        if (dm_fixed_list_info.Flags & DM_CATEGORY_CONTROL_COORDINATES)
        {
            show_fixed_list();
        }
        else
        {
            move_fixed_list(dm_fixed_list_info.S16X, dm_fixed_list_info.S16Y);
            if (dm_fixed_list_info.S16Width != MMI_fixed_list_menu.width ||
                dm_fixed_list_info.S16Height != MMI_fixed_list_menu.height)
            {
                resized = MMI_TRUE;
            }
            resize_fixed_list(dm_fixed_list_info.S16Width, dm_fixed_list_info.S16Height);
            if (resized == MMI_TRUE)
            {
                fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
            }
            show_fixed_list();
        }
    }
    else
    {
        show_fixed_list();
    }
    return 1;
}

/*****************************************************************************
* FUNCTION
*  dm_setup_and_draw_circular_menu
*
* DESCRIPTION
*   This function is used setup the dynamic list based on the coordinate set and
*  then draw it
*
* PARAMETERS
*  UICtrlAccessPtr_p IN/OUT   It specifies the start position to take out the coordinates.
*  *
* RETURNS                 
*  void
*
* GLOBALS AFFECTED        
*   
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_dynamic_list
 * DESCRIPTION
 *  This function is used setup the dynamic list based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_dynamic_list(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_dynamic_list_info;
    fixed_icontext_menuitem *menuitem_p;

     
    MMI_BOOL resized = MMI_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_dynamic_list_info);

    if (dm_dynamic_list_info.S16X == -1 || dm_dynamic_list_info.S16Y == -1 ||
        dm_dynamic_list_info.S16Width == -1 || dm_dynamic_list_info.S16Height == -1)
    {
        return 0;
    }

    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        move_fixed_list(dm_dynamic_list_info.S16X, dm_dynamic_list_info.S16Y);

        if (dm_dynamic_list_info.S16Width != MMI_fixed_list_menu.width ||
            dm_dynamic_list_info.S16Height != MMI_fixed_list_menu.height)
        {
            resized = MMI_TRUE;
        }
        resize_fixed_list(dm_dynamic_list_info.S16Width, dm_dynamic_list_info.S16Height);

        menuitem_p = (fixed_icontext_menuitem*) MMI_fixed_list_menu.common_item_data;
        gui_set_fixed_list_menu_common_item_data(&MMI_fixed_list_menu, menuitem_p);

        if (resized == MMI_TRUE)
        {
            dynamic_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
        }
         
    }
    show_dynamic_list();
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_asyncdynamic_list
 * DESCRIPTION
 *  This function is used setup the asyncdynamic list based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_asyncdynamic_list(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_dynamic_list_info;
    fixed_icontext_menuitem *menuitem_p;

     
    MMI_BOOL resized = MMI_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_dynamic_list_info);

    if (dm_dynamic_list_info.S16X == -1 || dm_dynamic_list_info.S16Y == -1 ||
        dm_dynamic_list_info.S16Width == -1 || dm_dynamic_list_info.S16Height == -1)
    {
        return 0;
    }
    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        move_fixed_list(dm_dynamic_list_info.S16X, dm_dynamic_list_info.S16Y);

        if (dm_dynamic_list_info.S16Width != MMI_fixed_list_menu.width ||
            dm_dynamic_list_info.S16Height != MMI_fixed_list_menu.height)
        {
            resized = MMI_TRUE;
        }
        resize_fixed_list(dm_dynamic_list_info.S16Width, dm_dynamic_list_info.S16Height);

        menuitem_p = (fixed_icontext_menuitem*) MMI_fixed_list_menu.common_item_data;
        gui_set_fixed_list_menu_common_item_data(&MMI_fixed_list_menu, menuitem_p);

        if (resized == MMI_TRUE)
        {
            asyncdynamic_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
        }
         
    }
    show_asyncdynamic_list();
    return 1;
}
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_320X240__) || defined(__PROJECT_GALLITE_C01__)
GDI_HANDLE  pda_layer_handle;
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */
/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_matrix_menu
 * DESCRIPTION
 *  This function is used setup the matrix menu based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_matrix_menu(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_matrix_menu_info;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_matrix_menu_info);
    /*+ zhouqin 2011-02-16 modify for q6 320X240 */
    #if defined(__MMI_MAINLCD_320X240__) || defined(__PROJECT_GALLITE_C01__)
    gdi_push_and_set_alpha_blending_source_layer(pda_layer_handle);
    #endif
    /*- zhouqin 2011-02-16 modify for q6 320X240 */
    if (dm_matrix_menu_info.S16X == -1 || dm_matrix_menu_info.S16Y == -1 ||
        dm_matrix_menu_info.S16Width == -1 || dm_matrix_menu_info.S16Height == -1)
    {
        return 0;
    }
    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        move_fixed_matrix(dm_matrix_menu_info.S16X, dm_matrix_menu_info.S16Y);
        if (fixed_matrix_auto_disable_scrollbar())
        {
            reset_fixed_matrix_col_rows();
        }
        resize_fixed_matrix(dm_matrix_menu_info.S16Width, dm_matrix_menu_info.S16Height);
    }
    show_fixed_matrix();
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_multiline_inputbox
 * DESCRIPTION
 *  This function is used setup the multiline inputbox based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  dm_cat_scr_info         [?]             
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_multiline_inputbox(S16 **UICtrlAccessPtr_p, dm_cat_scr_info_struct *dm_cat_scr_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_multiline_inputbox_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_multiline_inputbox_info);

    if (!get_multiline_inputbox_display_status())
    {
        return 0;
    }

    if (dm_multiline_inputbox_info.S16X == -1 || dm_multiline_inputbox_info.S16Y == -1 ||
        dm_multiline_inputbox_info.S16Width == -1 || dm_multiline_inputbox_info.S16Height == -1)
    {
        return 0;
    }
    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        if (dm_multiline_inputbox_info.Flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
        {
            S32 w = 0, h = 0;

            resize_multiline_inputbox_fit(
                dm_multiline_inputbox_info.S16Width,
                dm_multiline_inputbox_info.S16Height,
                &w,
                &h);
        }
        if (!dm_aligned_area_data.is_in_aligned_area)
        {
            if (wgui_inputbox_information_flag)
            {
            #ifdef UI_SMALL_CATEGORY_EDITOR
 
                if (whether_no_small_screen())
                {
                    wgui_setup_input_information(
                        dm_multiline_inputbox_info.S16X + 1,
                        dm_multiline_inputbox_info.S16Y,
                        dm_multiline_inputbox_info.S16Width - 2,
                        wgui_inputbox_information_bar_height);
                }
                else
            #endif /* UI_SMALL_CATEGORY_EDITOR */ 
 
                    wgui_setup_input_information(
                        dm_multiline_inputbox_info.S16X,
                        dm_multiline_inputbox_info.S16Y,
                        dm_multiline_inputbox_info.S16Width,
                        wgui_inputbox_information_bar_height);

                move_multiline_inputbox(
                    dm_multiline_inputbox_info.S16X,
                    dm_multiline_inputbox_info.S16Y + wgui_inputbox_information_bar_height);
                //Currently this is assuming that Multitap is always being shown whenever multiline
                //inputbox is shown, so this needs to be FIXED and appropriates condition needs to be checked here.
                if (!is_multiline_inputbox_in_view_mode())
                {
                #if defined(__MMI_FULL_SCREEN_EDITOR__)
                    if (dm_multiline_inputbox_info.Flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
                    {
                        dm_multiline_inputbox_info.S16Height = get_multiline_inputbox_height();
                    }
                    resize_multiline_inputbox(
                        dm_multiline_inputbox_info.S16Width,
                        dm_multiline_inputbox_info.S16Height - wgui_inputbox_information_bar_height);
                #else /* defined(__MMI_FULL_SCREEN_EDITOR__) */ 

                    if (dm_multiline_inputbox_info.Flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
                    {
                        dm_multiline_inputbox_info.S16Height = get_multiline_inputbox_height();
                    }
                    if (dm_multiline_inputbox_info.Flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT_NO_MULTITAP)
                    {
                        resize_multiline_inputbox(
                            dm_multiline_inputbox_info.S16Width,
                            dm_multiline_inputbox_info.S16Height - wgui_inputbox_information_bar_height);
                    }
                    else
                    {
                        resize_multiline_inputbox(
                            dm_multiline_inputbox_info.S16Width,
                            dm_multiline_inputbox_info.S16Height - wgui_inputbox_information_bar_height - MMI_multitap_height);
                    }
                    if (hide_multitap)
                    {
                        hide_multitap();    /* This is registered by Category to draw the area when multitap is now being shown. */
                    }
                #endif /* defined(__MMI_FULL_SCREEN_EDITOR__) */ 
                    dm_update_multitap_coordinates(&dm_multiline_inputbox_info, dm_cat_scr_info);
                }
                else
                {
                    if (dm_multiline_inputbox_info.Flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
                    {
                        dm_multiline_inputbox_info.S16Height = get_multiline_inputbox_height();
                    }
                    resize_multiline_inputbox(
                        dm_multiline_inputbox_info.S16Width,
                        dm_multiline_inputbox_info.S16Height - wgui_inputbox_information_bar_height);
                }
            }
            else
            {

                move_multiline_inputbox(dm_multiline_inputbox_info.S16X, dm_multiline_inputbox_info.S16Y);
                if (!is_multiline_inputbox_in_view_mode())
                {

                #if defined(__MMI_FULL_SCREEN_EDITOR__)

                    if (dm_multiline_inputbox_info.Flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
                    {
                        dm_multiline_inputbox_info.S16Height = get_multiline_inputbox_height();
                    }
                    resize_multiline_inputbox(
                        dm_multiline_inputbox_info.S16Width,
                        dm_multiline_inputbox_info.S16Height);
                #else /* defined(__MMI_FULL_SCREEN_EDITOR__) */ 
                    if (dm_multiline_inputbox_info.Flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
                    {
                        dm_multiline_inputbox_info.S16Height = get_multiline_inputbox_height();
                    }
                    if (dm_multiline_inputbox_info.Flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT_NO_MULTITAP)
                    {
                        resize_multiline_inputbox(
                            dm_multiline_inputbox_info.S16Width,
                            dm_multiline_inputbox_info.S16Height);
                    }
                    else
                    {
                        resize_multiline_inputbox(
                            dm_multiline_inputbox_info.S16Width,
                            dm_multiline_inputbox_info.S16Height - MMI_multitap_height);
                    }
                    if (hide_multitap)
                    {
                        hide_multitap();    /* This is registered by Category to draw the area when multitap is now being shown. */
                    }
                #endif /* defined(__MMI_FULL_SCREEN_EDITOR__) */ 
                    dm_update_multitap_coordinates(&dm_multiline_inputbox_info, dm_cat_scr_info);
                }
                else
                {
                    if (dm_multiline_inputbox_info.Flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
                    {
                        dm_multiline_inputbox_info.S16Height = get_multiline_inputbox_height();
                    }
                    resize_multiline_inputbox(
                        dm_multiline_inputbox_info.S16Width,
                        dm_multiline_inputbox_info.S16Height);
                }
            }
            setup_previous_control(&dm_multiline_inputbox_info);

        }       /* ( ! dm_aligned_area_data.is_in_aligned_area ) */
        else    /* ( dm_aligned_area_data.is_in_aligned_area ) */
        {
            dm_aligned_area_data.S16Y += dm_aligned_area_data.y_space;
            move_multiline_inputbox(dm_aligned_area_data.S16X, dm_aligned_area_data.S16Y);
            dm_aligned_area_data.S16Y += get_multiline_inputbox_height();
        }   /* ( dm_aligned_area_data.is_in_aligned_area ) */
    }
    else if (dm_aligned_area_data.is_in_aligned_area)
    {
        dm_aligned_area_data.S16Y += dm_aligned_area_data.y_space;
        dm_aligned_area_data.S16Y += get_multiline_inputbox_height();
    }
    wgui_show_inputbox();
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_ems_inputbox
 * DESCRIPTION
 *  This function is used setup the EMS inputbox based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  dm_cat_scr_info         [?]             
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_ems_inputbox(S16 **UICtrlAccessPtr_p, dm_cat_scr_info_struct *dm_cat_scr_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_multiline_inputbox_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_multiline_inputbox_info);

    if (!get_multiline_inputbox_display_status())
    {
        return 0;
    }

    if (dm_multiline_inputbox_info.S16X == -1 || dm_multiline_inputbox_info.S16Y == -1 ||
        dm_multiline_inputbox_info.S16Width == -1 || dm_multiline_inputbox_info.S16Height == -1)
    {
        return 0;
    }

    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        if (!dm_aligned_area_data.is_in_aligned_area)
        {
            if (wgui_inputbox_information_flag)
            {
                wgui_EMS_setup_input_information(
                    dm_multiline_inputbox_info.S16X,
                    dm_multiline_inputbox_info.S16Y,
                    dm_multiline_inputbox_info.S16Width,
                    wgui_EMS_inputbox_information_bar_height);
                move_EMS_inputbox(
                    dm_multiline_inputbox_info.S16X,
                    dm_multiline_inputbox_info.S16Y + wgui_EMS_inputbox_information_bar_height);
                //Currently this is assuming that Multitap is always being shown whenever multiline
                //inputbox is shown, so this needs to be FIXED and appropriates condition needs to be checked here.
                if (!is_EMS_inputbox_in_view_mode())
                {

                #if defined(__MMI_FULL_SCREEN_EDITOR__)

                    if (g_dm_data.S32flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
                    {
                        resize_EMS_inputbox(
                            dm_multiline_inputbox_info.S16Width,
                            get_multiline_inputbox_height() - wgui_inputbox_information_bar_height);
                    }
                    else
                    {
                        resize_EMS_inputbox(
                            dm_multiline_inputbox_info.S16Width,
                            dm_multiline_inputbox_info.S16Height - wgui_inputbox_information_bar_height);
                    }
                #else /* defined(__MMI_FULL_SCREEN_EDITOR__) */ 
                    if (g_dm_data.S32flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
                    {
                        resize_EMS_inputbox(
                            dm_multiline_inputbox_info.S16Width,
                            get_multiline_inputbox_height() - wgui_inputbox_information_bar_height - MMI_multitap_height);
                    }
                    else
                    {
                        resize_EMS_inputbox(
                            dm_multiline_inputbox_info.S16Width,
                            dm_multiline_inputbox_info.S16Height - wgui_inputbox_information_bar_height - MMI_multitap_height);
                    }
                    if (hide_multitap)
                    {
                        hide_multitap();    /* This is registered by Category to draw the area when multitap is now being shown. */
                    }
                #endif /* defined(__MMI_FULL_SCREEN_EDITOR__) */ 
                    dm_update_multitap_coordinates(&dm_multiline_inputbox_info, dm_cat_scr_info);
                }
                else
                {
                    if (g_dm_data.S32flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
                    {
                        resize_EMS_inputbox(
                            dm_multiline_inputbox_info.S16Width,
                            get_multiline_inputbox_height() - wgui_inputbox_information_bar_height);
                    }
                    else
                    {
                        resize_EMS_inputbox(
                            dm_multiline_inputbox_info.S16Width,
                            dm_multiline_inputbox_info.S16Height - wgui_inputbox_information_bar_height);
                    }
                }
            }
            else
            {

                move_EMS_inputbox(dm_multiline_inputbox_info.S16X, dm_multiline_inputbox_info.S16Y);
                if (!is_EMS_inputbox_in_view_mode())
                {

                #if defined(__MMI_FULL_SCREEN_EDITOR__)

                    if (g_dm_data.S32flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
                    {
                        resize_EMS_inputbox(dm_multiline_inputbox_info.S16Width, get_multiline_inputbox_height());
                    }
                    else
                    {
                        resize_EMS_inputbox(dm_multiline_inputbox_info.S16Width, dm_multiline_inputbox_info.S16Height);
                    }
                #else /* defined(__MMI_FULL_SCREEN_EDITOR__) */ 
                    if (g_dm_data.S32flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
                    {
                        resize_EMS_inputbox(
                            dm_multiline_inputbox_info.S16Width,
                            get_multiline_inputbox_height() - MMI_multitap_height);
                    }
                    else
                    {
                        resize_EMS_inputbox(
                            dm_multiline_inputbox_info.S16Width,
                            dm_multiline_inputbox_info.S16Height - MMI_multitap_height);
                    }
                    if (hide_multitap)
                    {
                        hide_multitap();    /* This is registered by Category to draw the area when multitap is now being shown. */
                    }
                #endif /* defined(__MMI_FULL_SCREEN_EDITOR__) */ 
                    dm_update_multitap_coordinates(&dm_multiline_inputbox_info, dm_cat_scr_info);
                }
                else
                {
                    if (g_dm_data.S32flags & DM_FIXED_MULTILINE_INPUTBOX_HEIGHT)
                    {
                        resize_EMS_inputbox(dm_multiline_inputbox_info.S16Width, get_multiline_inputbox_height());
                    }
                    else
                    {
                        resize_EMS_inputbox(dm_multiline_inputbox_info.S16Width, dm_multiline_inputbox_info.S16Height);
                    }
                }
            }
        }       /* ( ! dm_aligned_area_data.is_in_aligned_area ) */
        else    /* ( dm_aligned_area_data.is_in_aligned_area ) */
        {
            dm_aligned_area_data.S16Y += dm_aligned_area_data.y_space;
            move_EMS_inputbox(dm_aligned_area_data.S16X, dm_aligned_area_data.S16Y);
            dm_aligned_area_data.S16Y += get_multiline_inputbox_height();
        }   /* ( dm_aligned_area_data.is_in_aligned_area ) */
    }
    /* category5_hide_multitap(); */
     
    if (wgui_inputbox_information_flag)
    {
        wgui_EMS_redraw_input_information_bar();
    }
    show_EMS_inputbox();
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_dialer_inputbox
 * DESCRIPTION
 *  This function is used setup the dialer inputbox based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_dialer_inputbox(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_dialer_inputbox_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_dialer_inputbox_info);

    if (dm_dialer_inputbox_info.S16X == -1 || dm_dialer_inputbox_info.S16Y == -1 ||
        dm_dialer_inputbox_info.S16Width == -1 || dm_dialer_inputbox_info.S16Height == -1)
    {
        return 0;
    }
    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        move_dialer_inputbox(dm_dialer_inputbox_info.S16X, dm_dialer_inputbox_info.S16Y);
        resize_dialer_inputbox(dm_dialer_inputbox_info.S16Width, dm_dialer_inputbox_info.S16Height);
    }
    show_dialer_inputbox();
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_singleline_inputbox
 * DESCRIPTION
 *  This function is used setup the singleline inputbox based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_singleline_inputbox(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_singleline_inputbox_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_singleline_inputbox_info);

    if (dm_singleline_inputbox_info.S16X == -1 || dm_singleline_inputbox_info.S16Y == -1 ||
        dm_singleline_inputbox_info.S16Width == -1 || dm_singleline_inputbox_info.S16Height == -1)
    {
        return 0;
    }
    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        if (!dm_aligned_area_data.is_in_aligned_area)
        {
            move_singleline_inputbox(dm_singleline_inputbox_info.S16X, dm_singleline_inputbox_info.S16Y);
            if (dm_singleline_inputbox_info.Flags & DM_SINGLE_LINE_INPUTBOX_SPECIFIC_HEIGHT)
            {
                resize_singleline_inputbox(dm_singleline_inputbox_info.S16Width, dm_singleline_inputbox_info.S16Height);
            }
        }
        else
        {
            dm_aligned_area_data.S16Y += dm_aligned_area_data.y_space;
            move_singleline_inputbox(dm_aligned_area_data.S16X, dm_aligned_area_data.S16Y);
            dm_aligned_area_data.S16Y += MMI_singleline_inputbox_height;
        }
    }
    show_singleline_inputbox();
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_left_softkey
 * DESCRIPTION
 *  This function is used setup the left softkey based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_left_softkey(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_icontext_button_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_icontext_button_info);

    if (dm_icontext_button_info.S16X == -1 || dm_icontext_button_info.S16Y == -1 ||
        dm_icontext_button_info.S16Width == -1 || dm_icontext_button_info.S16Height == -1)
    {
        return 0;
    }
    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        move_softkey(dm_icontext_button_info.S16X, dm_icontext_button_info.S16Y, MMI_LEFT_SOFTKEY);
        resize_left_softkey(dm_icontext_button_info.S16Width, dm_icontext_button_info.S16Height);
    }
    show_left_softkey();
    return 1;

}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_right_softkey
 * DESCRIPTION
 *  This function is used setup the right softkey based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_right_softkey(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_icontext_button_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_icontext_button_info);

    if (dm_icontext_button_info.S16X == -1 || dm_icontext_button_info.S16Y == -1 ||
        dm_icontext_button_info.S16Width == -1 || dm_icontext_button_info.S16Height == -1)
    {
        return 0;
    }
    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        move_softkey(dm_icontext_button_info.S16X, dm_icontext_button_info.S16Y, MMI_RIGHT_SOFTKEY);
        resize_right_softkey(dm_icontext_button_info.S16Width, dm_icontext_button_info.S16Height);
    }
    show_right_softkey();
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_title
 * DESCRIPTION
 *  This function is used setup the title bar  based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_title(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_title_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_title_info);
    if (dm_title_info.S16X == -1 || dm_title_info.S16Y == -1 ||
        dm_title_info.S16Width == -1 || dm_title_info.S16Height == -1)
    {
        menu_shortcut_handler_display = 1;
        //MMI_title_string = NULL;
        //MMI_title_icon = NULL;
        return 0;
    }
    if ((g_dm_data.S32flags & DM_NO_TITLE))
    {
        return 0;
    }

    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        move_title(dm_title_info.S16X, dm_title_info.S16Y);
        resize_title(dm_title_info.S16Width, dm_title_info.S16Height);
 
        move_menu_shortcut_handler(MMI_title_x + MMI_title_width - MMI_menu_shortcut_box.width, MMI_title_y);    
  

    }

    if (g_dm_data.S32flags & DM_LEFT_ALIGN_TITLE)
    {
        draw_title_left_align();
    }
    else if (g_dm_data.S32flags & DM_SCROLL_TITLE)
    {
        draw_scrolling_title();
    }
    else
    {
        draw_title();
    }
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_horizontal_tab_bar
 * DESCRIPTION
 *  This function is used setup the horizontal tab bar based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
 	#ifdef __MMI_UI_TAB_PANE__

S32 dm_setup_and_draw_horizontal_tab_bar(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_htab_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_htab_info);

    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        wgui_move_horizontal_tab_bar(dm_htab_info.S16X, dm_htab_info.S16Y);
        move_title(dm_htab_info.S16X, dm_htab_info.S16Y + MMI_HORIZONTAL_TAB_BAR_TAB_AREA_HEIGHT);
        resize_title(dm_htab_info.S16Width, MMI_HORIZONTAL_TAB_BAR_HINT_AREA_HEIGHT);
    }
    wgui_show_horizontal_tab_bar();
    return 1;
}
#endif

/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_category_controlled_area
 * DESCRIPTION
 *  This function is used setup the category controlled area based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_category_controlled_area(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_category_controlled_area_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_category_controlled_area_info);
    if (dm_category_controlled_area_info.S16X == -1 || dm_category_controlled_area_info.S16Y == -1 ||
        dm_category_controlled_area_info.S16Width == -1 || dm_category_controlled_area_info.S16Height == -1)
    {
        return 0;
    }
#ifdef __MMI_SUBLCD__
    if (g_dm_data.S32flags & DM_SUB_LCD_CONTEXT)
    {
        if (dm_sub_category_controlled_area_callback)
        {
            (*dm_sub_category_controlled_area_callback) (&dm_category_controlled_area_info);
        }
    }
    else
#endif /* __MMI_SUBLCD__ */ 
    {
        if (dm_category_controlled_area_callback)
        {
            (*dm_category_controlled_area_callback) (&dm_category_controlled_area_info);
        }
    }
    setup_previous_control(&dm_category_controlled_area_info);

    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_category_controlled_area2
 * DESCRIPTION
 *  This function is used setup the category controlled area2 based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_category_controlled_area2(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_category_controlled_area_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_category_controlled_area_info);
    if (dm_category_controlled_area_info.S16X == -1 || dm_category_controlled_area_info.S16Y == -1 ||
        dm_category_controlled_area_info.S16Width == -1 || dm_category_controlled_area_info.S16Height == -1)
    {
        return 0;
    }
    if (dm_category_controlled_area2_callback)
    {
        (*dm_category_controlled_area2_callback) (&dm_category_controlled_area_info);
    }

    setup_previous_control(&dm_category_controlled_area_info);

    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_status_icons
 * DESCRIPTION
 *  This function is used setup the status icons based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_status_icons(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_status_info_struct dm_status_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_status_icon_bar_coordinates(*UICtrlAccessPtr_p, &dm_status_info);

    if (dm_status_info.x == -1 || dm_status_info.y == -1 ||
        dm_status_info.x1 == -1 || dm_status_info.y1 == -1 || dm_status_info.x2 == -1 || dm_status_info.y2 == -1)
    {
        return 0;
    }
    if ((g_dm_data.S32flags & DM_NO_STATUS_BAR))
    {
        return 0;
    }

#ifdef __MMI_SUBLCD__
    if (g_dm_data.S32flags & DM_SUB_LCD_CONTEXT)
    {
        SUBLCD_screens_show_status_icons();
    }
    else
#endif /* __MMI_SUBLCD__ */ 
    {
 /*+ zhouqin 2011-02-16 modify for q6 320X240 */
    #if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)  ||defined(__MMI_MAINLCD_320X240__)    
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	if ((g_dm_data.S32flags & DM_SPECIFIC_HIDE_STATUS_BAR))
        {
            show_status_icons();
        }
        else
        {
            show_title_status_icon();
        }
    #else /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__) ||defined(__MMI_MAINLCD_220X176__)*/ 
        show_status_icons();
    #endif /* defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__) */ 
    }
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_button_bar
 * DESCRIPTION
 *  This function is used setup the button bar based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_button_bar(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_buttonbar_info_struct dm_buttonbar_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_button_bar_coordinates(*UICtrlAccessPtr_p, &dm_buttonbar_info);

    if (dm_buttonbar_info.coordinate.S16X == -1 || dm_buttonbar_info.coordinate.S16Y == -1 ||
        dm_buttonbar_info.coordinate.S16Width == -1 || dm_buttonbar_info.coordinate.S16Height == -1 ||
        dm_buttonbar_info.button_width == -1)
    {
        return 0;
    }

    if (g_dm_data.S32flags & DM_NO_SOFTKEY)
    {
        return 0;
    }

    if (g_dm_cntx.redraw_screen_main == REDRAW_2_REDRAW)
    {
        if (!(dm_buttonbar_info.coordinate.Flags & DM_BUTTON_DISABLE_BACKGROUND))
        {
            show_softkey_background();
		/*QPAD arrow indications are filled by red(Fixed in the softkey_background). 
		If you need special color you can modify them in the file(ThemeConverterUI.c  list_selected_item_filler variable)*/
            draw_arrow_indication(
                (S8) (dm_buttonbar_info.coordinate.Flags & DM_BUTTON_BAR_LEFT_ARROW),
                (S8) (dm_buttonbar_info.coordinate.Flags & DM_BUTTON_BAR_RIGHT_ARROW),
                (S8) (dm_buttonbar_info.coordinate.Flags & DM_BUTTON_BAR_UP_ARROW),
                (S8) (dm_buttonbar_info.coordinate.Flags & DM_BUTTON_BAR_DOWN_ARROW));
        }
        if (!(g_dm_data.S32flags & DM_NO_SOFTKEY))
        {
            show_softkey(MMI_LEFT_SOFTKEY);
            show_softkey(MMI_RIGHT_SOFTKEY);
        #ifdef __MMI_WGUI_CSK_ENABLE__
            show_softkey(MMI_CENTER_SOFTKEY);
        #endif 
        }
        return 1;
    }

    gui_reset_clip();

    set_button_bar_height(dm_buttonbar_info.coordinate.S16Height);
    if (!(dm_buttonbar_info.coordinate.Flags & DM_BUTTON_DISABLE_BACKGROUND))
    {
        show_softkey_background();
#if defined(__T800_MAIN_MENU__)||defined(__MMI_MAINLCD_128X128__)
        draw_arrow_indication(
            (S8) (dm_buttonbar_info.coordinate.Flags & DM_BUTTON_BAR_LEFT_ARROW),
            (S8) (dm_buttonbar_info.coordinate.Flags & DM_BUTTON_BAR_RIGHT_ARROW),
            (S8) (dm_buttonbar_info.coordinate.Flags & DM_BUTTON_BAR_UP_ARROW),
            (S8) (dm_buttonbar_info.coordinate.Flags & DM_BUTTON_BAR_DOWN_ARROW));
#else
	/*QPAD arrow indications are filled by red(Fixed in the softkey_background). 
	If you need special color you can modify them in the file(ThemeConverterUI.c  list_selected_item_filler variable)*/
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_MAINLCD_128X128__)
        draw_arrow_indication(
            (S8) (dm_buttonbar_info.coordinate.Flags & DM_BUTTON_BAR_LEFT_ARROW),
            (S8) (dm_buttonbar_info.coordinate.Flags & DM_BUTTON_BAR_RIGHT_ARROW),
            (S8) (dm_buttonbar_info.coordinate.Flags & DM_BUTTON_BAR_UP_ARROW),
            (S8) (dm_buttonbar_info.coordinate.Flags & DM_BUTTON_BAR_DOWN_ARROW));
#endif
#endif
    }

    if (!(g_dm_data.S32flags & DM_NO_SOFTKEY))
    {
    #ifdef __MMI_SCREEN_ROTATE__
        if (mmi_frm_is_screen_width_height_swapped())
        {
            /* Vertical softkey bar aligned to right  */
            /* FIXME. replace fixed value MMI_ROTATED_SOFTKEY_HEIGHT */
            set_softkey_flags_on(UI_BUTTON_RIGHT_ALIGN, MMI_LEFT_SOFTKEY);
            set_softkey_flags_off(UI_BUTTON_LEFT_ALIGN, MMI_LEFT_SOFTKEY);
            move_softkey(
                dm_buttonbar_info.coordinate.S16X,
                dm_buttonbar_info.coordinate.S16Y + dm_buttonbar_info.coordinate.S16Height - MMI_ROTATED_SOFTKEY_HEIGHT,
                MMI_LEFT_SOFTKEY);
            resize_softkey(dm_buttonbar_info.button_width, MMI_ROTATED_SOFTKEY_HEIGHT, MMI_LEFT_SOFTKEY);
            show_softkey(MMI_LEFT_SOFTKEY);

            move_softkey(dm_buttonbar_info.coordinate.S16X, dm_buttonbar_info.coordinate.S16Y, MMI_RIGHT_SOFTKEY);
            resize_softkey(dm_buttonbar_info.button_width, MMI_ROTATED_SOFTKEY_HEIGHT, MMI_RIGHT_SOFTKEY);
            show_softkey(MMI_RIGHT_SOFTKEY);

        #ifdef __MMI_WGUI_CSK_ENABLE__
            set_softkey_flags_on(UI_BUTTON_RIGHT_ALIGN, MMI_CENTER_SOFTKEY);
            set_softkey_flags_off(UI_BUTTON_LEFT_ALIGN, MMI_LEFT_SOFTKEY);
            move_softkey(
                dm_buttonbar_info.coordinate.S16X,
                dm_buttonbar_info.coordinate.S16Y + MMI_ROTATED_SOFTKEY_HEIGHT + ((dm_buttonbar_info.coordinate.S16Height - MMI_ROTATED_SOFTKEY_HEIGHT * 3) >> 1),
                MMI_CENTER_SOFTKEY);
            resize_softkey(dm_buttonbar_info.button_width, MMI_ROTATED_SOFTKEY_HEIGHT, MMI_CENTER_SOFTKEY);
            show_softkey(MMI_CENTER_SOFTKEY);
        #endif /* __MMI_WGUI_CSK_ENABLE__ */ 
        }
        else
    #endif /* __MMI_SCREEN_ROTATE__ */ 
        {
 
        #ifdef __MMI_UI_DALMATIAN_SOFTKEYS__
            dm_buttonbar_info.coordinate.S16Height -= (MMI_BUTTON_BAR_HEIGHT - MMI_SOFTKEY_HEIGHT);
            move_softkey(
                dm_buttonbar_info.coordinate.S16X,
                dm_buttonbar_info.coordinate.S16Y + (MMI_BUTTON_BAR_HEIGHT - MMI_SOFTKEY_HEIGHT),
                MMI_LEFT_SOFTKEY);
        #else /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
            move_softkey(dm_buttonbar_info.coordinate.S16X, dm_buttonbar_info.coordinate.S16Y, MMI_LEFT_SOFTKEY);
        #endif /* __MMI_UI_DALMATIAN_SOFTKEYS__ */ 
 
            resize_softkey(dm_buttonbar_info.button_width, dm_buttonbar_info.coordinate.S16Height, MMI_LEFT_SOFTKEY);
            show_softkey(MMI_LEFT_SOFTKEY);

            move_softkey(
                dm_buttonbar_info.coordinate.S16X + dm_buttonbar_info.coordinate.S16Width - dm_buttonbar_info.button_width,
                dm_buttonbar_info.coordinate.S16Y,
                MMI_RIGHT_SOFTKEY);
            resize_softkey(dm_buttonbar_info.button_width, dm_buttonbar_info.coordinate.S16Height, MMI_RIGHT_SOFTKEY);
            show_softkey(MMI_RIGHT_SOFTKEY);

        #ifdef __MMI_WGUI_CSK_ENABLE__
            move_softkey(
                dm_buttonbar_info.coordinate.S16X + dm_buttonbar_info.button_width + ((dm_buttonbar_info.coordinate.S16Width - (dm_buttonbar_info.button_width * 3)) >> 1),
                dm_buttonbar_info.coordinate.S16Y,
                MMI_CENTER_SOFTKEY);
            resize_softkey(dm_buttonbar_info.button_width, dm_buttonbar_info.coordinate.S16Height, MMI_CENTER_SOFTKEY);
            show_softkey(MMI_CENTER_SOFTKEY);
        #endif /* __MMI_WGUI_CSK_ENABLE__ */ 
        }
    }
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_inline_item
 * DESCRIPTION
 *  This function is used setup the inline items based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_inline_item(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_inline_fixed_list_info;
    fixed_icontext_menuitem *menuitem_p;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_inline_fixed_list_info);

    if (dm_inline_fixed_list_info.S16X == -1 || dm_inline_fixed_list_info.S16Y == -1 ||
        dm_inline_fixed_list_info.S16Width == -1 || dm_inline_fixed_list_info.S16Height == -1)
    {
        return 0;
    }
    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        gui_set_font(&MMI_medium_font);
        MMI_fixed_icontext_menuitem.text_font = &MMI_medium_font;
        MMI_fixed_text_menuitem.text_font = &MMI_medium_font;

        move_fixed_list(dm_inline_fixed_list_info.S16X, dm_inline_fixed_list_info.S16Y);
        resize_fixed_list(dm_inline_fixed_list_info.S16Width, dm_inline_fixed_list_info.S16Height);
         
        if ((g_dm_data.S32flags & DM_SHOW_VKPAD))
        {
        #if defined(__MMI_TOUCH_SCREEN__)
            mmi_pen_editor_store_list_menu_height(MMI_fixed_list_menu.height);
            if ((GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY != MMI_virtual_keyboard.lang_type)
                && (!((current_wgui_inline_item->flags & INLINE_ITEM_TYPE_MASK) == INLINE_ITEM_TYPE_FULL_SCREEN_EDIT)))
            {
                /*
                resize_fixed_list(
                    MMI_content_width,
                    mmi_pen_editor_get_list_menu_height() - MMI_virtual_keyboard.height - 1);
                */
                mmi_pen_editor_list_menu_highlight_resize();
                 
                gui_fixed_list_menu_locate_highlighted_item(&MMI_fixed_list_menu);
                if ((current_wgui_inline_item->flags & INLINE_ITEM_TYPE_MASK) == INLINE_ITEM_TYPE_MULTI_LINE_EDIT)
                {
                    MMI_fixed_list_menu.first_displayed_item = MMI_fixed_list_menu.highlighted_item;
                }
            }
        #endif /* defined(__MMI_TOUCH_SCREEN__) */ 
        }

        menuitem_p = (fixed_icontext_menuitem*) MMI_fixed_list_menu.common_item_data;

        /* menuitem_p->width = dm_inline_fixed_list_info.S16Width; */
        gui_set_fixed_list_menu_common_item_data(&MMI_fixed_list_menu, menuitem_p);
        move_fixed_list(dm_inline_fixed_list_info.S16X, dm_inline_fixed_list_info.S16Y);
    }

    show_fixed_list();
    redraw_current_inline_item();
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_base_control_set
 * DESCRIPTION
 *  This function is used setup the base control set based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  _UICtrlAccessPtr            [IN]            
 *  UICtrlAccessPtr_p(?)        [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_base_control_set(S16 **_UICtrlAccessPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *control_set_ptr;
    U8 u8CtrlCt = 0;
    U8 u8NoOfUICtrls = 0;
    S16 *UICtrlAccessPtr_p = NULL;

#ifdef __MMI_SCREEN_ROTATE__
    MMI_BOOL popup_in_rotated_screen = FALSE;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_SCREEN_ROTATE__
    /*
     * 1. Draw a border around title and content area. 
     * 2. Clear content area because some category screen (e.g. 66) does not clear it.
     *
     * If a category screen do not want to use standard MMI_ROTATED_{CONTENT|TITLE}_{X|Y}, 
     * it should not contain DM_BASE_CONTROL_SET1 in its control set.
     */
    if (mmi_frm_is_screen_width_height_swapped() && !wgui_is_wallpaper_on_bottom())
    {
        popup_in_rotated_screen = dm_check_control_exist(g_dm_data.S32CatId, DM_POPUP_BACKGROUND);
        if (!popup_in_rotated_screen && /* Prevent to erase popup background here */
            !(g_dm_data.S32flags & DM_NO_TITLE))
        {
            gui_push_clip();
            gui_reset_clip();
            gui_fill_rectangle(
                MMI_ROTATED_TITLE_X - 1,
                MMI_ROTATED_TITLE_Y - 1,
                MMI_ROTATED_TITLE_X + MMI_ROTATED_TITLE_WIDTH,
                MMI_ROTATED_TITLE_Y + MMI_ROTATED_TITLE_HEIGHT + MMI_ROTATED_CONTENT_HEIGHT,
                gui_color(255, 255, 255));
            gui_pop_clip();
        }
    }
#endif /* __MMI_SCREEN_ROTATE__ */ 

    control_set_ptr = dm_get_base_control_set();
    UICtrlAccessPtr_p = dm_get_base_coordinate_set();
    u8NoOfUICtrls = control_set_ptr[0];
    for (u8CtrlCt = 1; u8CtrlCt <= u8NoOfUICtrls; u8CtrlCt++)
    {
        switch (control_set_ptr[u8CtrlCt])
        {
            case DM_TITLE1:
            {
                dm_setup_and_draw_title(&UICtrlAccessPtr_p);
                break;
            }
            case DM_STATUS_BAR1:
            {
                dm_setup_and_draw_status_icons(&UICtrlAccessPtr_p);
                break;
            }
            case DM_BUTTON_BAR1:
            {
                dm_setup_and_draw_button_bar(&UICtrlAccessPtr_p);
                break;
            }
            case DM_SCR_BG:
        #ifdef __MMI_SCREEN_ROTATE__
                /* 
                 * When screen is rotated, we do not show screen background if popup is drawn.
                 * It simplify the design because partial screen is transparent.
                 */
                if (!mmi_frm_is_screen_width_height_swapped() || !popup_in_rotated_screen)
        #endif /* __MMI_SCREEN_ROTATE__ */ 
                {
                    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
                    {
                        dm_setup_and_draw_scr_bg(&UICtrlAccessPtr_p);
                    }
                }
        }
    }

    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_base_control_set2
 * DESCRIPTION
 *  This function is used setup the base control set 2 based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  _UICtrlAccessPtr            [IN]            
 *  UICtrlAccessPtr_p(?)        [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_base_control_set2(S16 **_UICtrlAccessPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *control_set_ptr;
    U8 u8CtrlCt = 0;
    U8 u8NoOfUICtrls = 0;
    S16 *UICtrlAccessPtr_p = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    control_set_ptr = dm_get_base_control_set2();
    UICtrlAccessPtr_p = dm_get_base_coordinate_set2();
    u8NoOfUICtrls = control_set_ptr[0];
    for (u8CtrlCt = 1; u8CtrlCt <= u8NoOfUICtrls; u8CtrlCt++)
    {
        switch (control_set_ptr[u8CtrlCt])
        {
            case DM_STATUS_BAR1:
            {
                dm_setup_and_draw_status_icons(&UICtrlAccessPtr_p);
                break;
            }
            case DM_BUTTON_BAR1:
            {
                dm_setup_and_draw_button_bar(&UICtrlAccessPtr_p);
                break;
            }
            case DM_SCR_BG:
            {
                dm_setup_and_draw_scr_bg(&UICtrlAccessPtr_p);
            }
        }
    }
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_string
 * DESCRIPTION
 *  This function is used setup the string area set based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  index                   [IN]            
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_string(S16 **UICtrlAccessPtr_p, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_string_info;
    S32 string_width, string_height;
    dm_string_data_struct *dm_string;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_SUBLCD__
    if (g_dm_data.S32flags & DM_SUB_LCD_CONTEXT)
    {
        dm_string = &dm_string_array_sub[index];
    }
    else
#endif /* __MMI_SUBLCD__ */ 
        dm_string = &dm_string_array[index];

    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_string_info);
    if (dm_string_info.S16X == -1 || dm_string_info.S16Y == -1 ||
        dm_string_info.S16Width == -1 || dm_string_info.S16Height == -1)
    {
        return 0;
    }

    gui_measure_string(dm_string->string, &string_width, &string_height);

    if (dm_string->string == NULL || (string_width == 0 && string_height == 0))
    {
        return 0;
    }

    if (dm_string_info.S16Height == DM_CALCULATED_HEIGHT)
    {
        /* 060706 menstral Start */
        if ( dm_string_info.Flags & DM_STRING_BORDERED )
        {
            dm_string_info.S16Height=string_height+2;
        }
        else
        {
            dm_string_info.S16Height=string_height;
        }
        /* 060706 menstral End*/
        if (dm_string_info.Flags & DM_PREVIOUS_CONTROL_END_Y_OFFSET_HEIGHT)
        {
            dm_string_info.S16Y -= dm_string_info.S16Height;
        }
    }
    if (dm_string_info.S16Width == DM_CALCULATED_WIDTH)
    {
        dm_string_info.S16Width = string_width + 1;
        if (dm_string_info.Flags & DM_PREVIOUS_CONTROL_END_X_OFFSET_WIDTH)
        {
            dm_string_info.S16X -= dm_string_info.S16Width;
        }
    }

    gui_push_text_clip();
    gui_push_clip();

    if (!dm_aligned_area_data.is_in_aligned_area)
    {
        gui_set_clip(
            dm_string_info.S16X,
            dm_string_info.S16Y,
            dm_string_info.S16X + dm_string_info.S16Width - 1,
            dm_string_info.S16Y + dm_string_info.S16Height - 1);
        if (dm_string_info.Flags & DM_BACK_FILL)
        {
            gui_draw_filled_area(
                dm_string_info.S16X,
                dm_string_info.S16Y,
                dm_string_info.S16X + dm_string_info.S16Width - 1,
                dm_string_info.S16Y + dm_string_info.S16Height - 1,
                &(dm_string->back_fill));
        }

        gui_set_font(dm_string->font);
        gui_measure_string(dm_string->string, &string_width, &string_height);
        /* Align the string on x asis as per the flag */
        if (string_width < dm_string_info.S16Width)
        {
            if (dm_string_info.Flags & DM_CENTRE_ALIGN_X)
            {
                dm_string_info.S16X += (dm_string_info.S16Width - string_width) >> 1;
            }
            else if (dm_string_info.Flags & DM_RIGHT_ALIGN_X)
            {
                dm_string_info.S16X += (dm_string_info.S16Width - string_width);
            }
        }
        /* Add the string width to string x in case of right to left MMI. */
        if (r2lMMIFlag)
        {
            dm_string_info.S16X += string_width;
        }
        /* Align the string on y asis as per the flag */
        if (string_height < dm_string_info.S16Height)
        {
            if (dm_string_info.Flags & DM_CENTER_ALIGN_Y)
            {
                dm_string_info.S16Y += (dm_string_info.S16Height - string_height) >> 1;
            }
            else if (dm_string_info.Flags & DM_BOTTOM_ALIGN_Y)
            {
                dm_string_info.S16Y += (dm_string_info.S16Height - string_height);
            }
        }

        setup_previous_control(&dm_string_info);

        gui_set_text_clip(
            dm_string_info.S16X,
            dm_string_info.S16Y,
            dm_string_info.S16X + dm_string_info.S16Width - 1,
            dm_string_info.S16Y + dm_string_info.S16Height - 1);
        gui_set_text_color(dm_string->text_color);
        //gui_move_text_cursor(dm_string_info.S16X, dm_string_info.S16Y);
        if (dm_string_info.Flags & DM_STRING_BORDERED)
        {
            gui_move_text_cursor(dm_string_info.S16X, dm_string_info.S16Y+1);
            gui_set_text_border_color(dm_string->border_color);
            /* if(! (g_dm_data.S32flags & DM_NO_STRING) ) */
            /* 072106 dm string Start */
            if (string_width > dm_string_info.S16Width)
            {
                gui_print_truncated_borderd_text(dm_string_info.S16X, dm_string_info.S16Y+1, dm_string_info.S16Width, dm_string->string);
            }
            else
            {
                gui_print_bordered_text(dm_string->string);
            }
            /* 072106 dm string End */
        }
        else
        {
            /* if(! (g_dm_data.S32flags & DM_NO_STRING) ) */
            gui_move_text_cursor(dm_string_info.S16X, dm_string_info.S16Y);
            /* 072106 dm string Start */
            if (string_width > dm_string_info.S16Width)
            {
                gui_print_truncated_text(dm_string_info.S16X, dm_string_info.S16Y, dm_string_info.S16Width, dm_string->string);
            }
            else
            {
                gui_print_text(dm_string->string);
            }
            /* 072106 dm string End */
        }
        gui_pop_text_clip();
        gui_pop_clip();
        return 1;
    }
    else
    {
        gui_set_font(dm_string->font);
        gui_measure_string(dm_string->string, &string_width, &string_height);

        dm_string_info.S16X = dm_aligned_area_data.S16X;
        dm_string_info.S16Y = dm_aligned_area_data.S16Y;
        gui_set_clip(
            dm_aligned_area_data.S16X,
            dm_aligned_area_data.S16Y,
            dm_aligned_area_data.S16X + dm_aligned_area_data.S16width - 1,
            dm_aligned_area_data.S16Y + dm_aligned_area_data.S16height - 1);

        /* We should make sure background filler is not NULL. */
        if ((dm_string_info.Flags & DM_BACK_FILL) && (&(dm_string->back_fill) != NULL))
        {
            gui_draw_filled_area(
                dm_aligned_area_data.S16X,
                dm_aligned_area_data.S16X,
                dm_aligned_area_data.S16X + dm_aligned_area_data.S16width - 1,
                dm_aligned_area_data.S16Y + dm_aligned_area_data.S16height - 1,
                &(dm_string->back_fill));
        }

        /* Align the string on x asis as per the flag */
        if (string_width < dm_aligned_area_data.S16width)
        {
            if (dm_string_info.Flags & DM_CENTRE_ALIGN_X)
            {
                dm_string_info.S16X += (dm_aligned_area_data.S16width - string_width) >> 1;
            }
            else if (dm_string_info.Flags & DM_RIGHT_ALIGN_X)
            {
                dm_string_info.S16X += (dm_aligned_area_data.S16width - string_width);
            }
        }
        /* Add the string width to string x in case of right to left MMI. */
        if (r2lMMIFlag)
        {
            dm_string_info.S16X += string_width;
        }
        /* Align the string on y asis as per the flag */
        if (string_height < dm_string_info.S16Height)
        {
            if (dm_string_info.Flags & DM_CENTER_ALIGN_Y)
            {
                dm_string_info.S16Y += (dm_string_info.S16Height - string_height) >> 1;
            }
            else if (dm_string_info.Flags & DM_BOTTOM_ALIGN_Y)
            {
                dm_string_info.S16Y += (dm_string_info.S16Height - string_height);
            }
        }
        dm_string_info.S16Y += dm_aligned_area_data.y_space;     
        gui_set_text_clip(
            dm_string_info.S16X,
            dm_string_info.S16Y,
            dm_string_info.S16X + dm_string_info.S16Width - 1,
            dm_string_info.S16Y + dm_string_info.S16Height - 1);
        gui_set_text_color(dm_string->text_color);
        /* dm_string_info.S16Y+=dm_aligned_area_data.y_space; */
        dm_aligned_area_data.S16Y = dm_string_info.S16Y;
        gui_move_text_cursor(dm_string_info.S16X, dm_string_info.S16Y);
        if (dm_string_info.Flags & DM_STRING_BORDERED)
        {
            gui_set_text_border_color(dm_string->border_color);
            /* if(! (g_dm_data.S32flags & DM_NO_STRING) ) */
            /* 072106 dm string Start */
            if (string_width > dm_string_info.S16Width)
            {
                gui_print_truncated_borderd_text(dm_string_info.S16X, dm_string_info.S16Y, dm_string_info.S16Width, dm_string->string);
            }
            else
            {
                gui_print_bordered_text(dm_string->string);
            }
            /* 072106 dm string End */
        }
        else
        {
            /* if(! (g_dm_data.S32flags & DM_NO_STRING) ) */
            /* 072106 dm string Start */
            if (string_width > dm_string_info.S16Width)
            {
                gui_print_truncated_text(dm_string_info.S16X, dm_string_info.S16Y, dm_string_info.S16Width, dm_string->string);
            }
            else
            {
                gui_print_text(dm_string->string);
            }
            /* 072106 dm string End */
        }
        gui_pop_text_clip();
        gui_pop_clip();
        dm_aligned_area_data.S16Y += string_height;
        return 1;
    }
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_image
 * DESCRIPTION
 *  This function is used setup the image area set based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  index                   [IN]            
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_image(S16 **UICtrlAccessPtr_p, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_image_info;
    S32 img_w, img_h;
    dm_image_data_struct *dm_image;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_SUBLCD__
    if (g_dm_data.S32flags & DM_SUB_LCD_CONTEXT)
    {
        dm_image = &dm_image_array_sub[index];
    }
    else
#endif /* __MMI_SUBLCD__ */ 
        dm_image = &dm_image_array[index];

    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_image_info);
    if (dm_image_info.S16X == -1 || dm_image_info.S16Y == -1 ||
        dm_image_info.S16Width == -1 || dm_image_info.S16Height == -1)
    {
        return 0;
    }

    gui_measure_image(dm_image->image, &img_w, &img_h);
    if ((dm_image->image == NULL && dm_image->name == NULL) || (img_w == 0 && img_h == 0))
    {
        return 0;
    }

    if (dm_image_info.S16Height == DM_CALCULATED_HEIGHT)
    {
        dm_image_info.S16Height = img_h;
        if (dm_image_info.Flags & DM_PREVIOUS_CONTROL_END_Y_OFFSET_HEIGHT)
        {
            dm_image_info.S16Y -= dm_image_info.S16Height;
        }
    }
    if (dm_image_info.S16Width == DM_CALCULATED_WIDTH)
    {
        dm_image_info.S16Width = img_w;
        if (dm_image_info.Flags & DM_PREVIOUS_CONTROL_END_X_OFFSET_WIDTH)
        {
            dm_image_info.S16X -= dm_image_info.S16Width;
        }
    }

    if (!dm_aligned_area_data.is_in_aligned_area)
    {

        gui_push_clip();
        gui_set_clip(
            dm_image_info.S16X,
            dm_image_info.S16Y,
            dm_image_info.S16X + dm_image_info.S16Width - 1,
            dm_image_info.S16Y + dm_image_info.S16Height - 1);

        /* We should make sure background filler is not NULL */
        if ((dm_image_info.Flags & DM_BACK_FILL) && (&dm_image->back_fill != NULL))
        {
            gui_draw_filled_area(
                dm_image_info.S16X,
                dm_image_info.S16Y,
                dm_image_info.S16X + dm_image_info.S16Width - 1,
                dm_image_info.S16Y + dm_image_info.S16Height - 1,
                dm_image->back_fill);
        }

        gui_measure_image(dm_image->image, &img_w, &img_h);

        /* Align the image on x asis as per the flag */
        /* 062106 cat129 Start */
        if (dm_image_info.Flags & DM_CENTRE_ALIGN_X || dm_image_info.Flags & DM_RIGHT_ALIGN_X)
        {
            if (img_w < dm_image_info.S16Width)
            {
                if (dm_image_info.Flags & DM_CENTRE_ALIGN_X)
                {
                    dm_image_info.S16X += (dm_image_info.S16Width - img_w) >> 1;
                }
                else if (dm_image_info.Flags & DM_RIGHT_ALIGN_X)
                {
                    dm_image_info.S16X += (dm_image_info.S16Width - img_w);
                }
            }
        }
        else if (dm_image_info.Flags & DM_FORCE_CENTRE_ALIGN_X)
        {
            dm_image_info.S16X+=(dm_image_info.S16Width-img_w)>>1;
        }

        /* Align the image on y asis as per the flag */
        if (dm_image_info.Flags & DM_CENTER_ALIGN_Y || dm_image_info.Flags & DM_BOTTOM_ALIGN_Y)
        {
            if (img_h < dm_image_info.S16Height)
            {
                if (dm_image_info.Flags & DM_CENTER_ALIGN_Y)
                {
                    dm_image_info.S16Y += (dm_image_info.S16Height - img_h) >> 1;
                }
                else if (dm_image_info.Flags & DM_BOTTOM_ALIGN_Y)
                {
                    dm_image_info.S16Y += (dm_image_info.S16Height - img_h);
                }
            }
        }
        else if (dm_image_info.Flags & DM_FORCE_CENTER_ALIGN_Y)
        {
            dm_image_info.S16Y+=(dm_image_info.S16Height-img_h)>>1;
        }
        /* 062106 cat129 Start */

        setup_previous_control(&dm_image_info);

        if (dm_image->image_handle != GDI_ERROR_HANDLE)
        {
            gdi_image_stop_animation(dm_image->image_handle);
        }

        if (dm_image->name != NULL) /* from file */
        {
            gdi_image_draw_animation_file(
                dm_image_info.S16X,
                dm_image_info.S16Y,
                (S8*) dm_image->name,
                &(dm_image->image_handle));
        }
        else if (dm_image->image != NULL)   /* from resource */
        {
            gdi_image_draw_animation(
                dm_image_info.S16X,
                dm_image_info.S16Y,
                dm_image->image,
                &(dm_image->image_handle));
        }

        dm_image->x = dm_image_info.S16X;
        dm_image->y = dm_image_info.S16Y;
        dm_image->width = img_w;
        dm_image->height = img_h;

        gui_pop_clip();
        return 1;
    }
    else
    {
        S32 image_width = 0, image_height = 0;
        S32 x = 0, y = 0;

        gui_push_clip();
        gui_set_clip(
            dm_aligned_area_data.S16X,
            dm_aligned_area_data.S16Y,
            dm_aligned_area_data.S16X + dm_aligned_area_data.S16width - 1,
            dm_aligned_area_data.S16Y + dm_aligned_area_data.S16height - 1);

        dm_aligned_area_data.S16Y += dm_aligned_area_data.y_space;

        if (dm_image->name != NULL) /* from file */
        {
            gdi_image_get_dimension_file((S8*) dm_image->name, &image_width, &image_height);
            if (dm_aligned_area_data.S16width > image_width)
            {
                x = dm_aligned_area_data.S16X + ((dm_aligned_area_data.S16width - image_width) >> 1);
                y = dm_aligned_area_data.S16Y;
                gdi_image_draw_animation_file(x, y, (S8*) dm_image->name, &(dm_image->image_handle));
            }
            else
            {
                x = dm_aligned_area_data.S16X + ((dm_aligned_area_data.S16width - image_width) >> 1);
                y = dm_aligned_area_data.S16Y;
                gdi_image_draw_animation_file(x, y, (S8*) dm_image->name, &(dm_image->image_handle));
            }
        }
        else if (dm_image->image != NULL)   /* from resource */
        {
            gdi_image_get_dimension(dm_image->image, &image_width, &image_height);
            if (dm_aligned_area_data.S16width > image_width)
            {
                x = dm_aligned_area_data.S16X + ((dm_aligned_area_data.S16width - image_width) >> 1);
                y = dm_aligned_area_data.S16Y;
                gdi_image_draw_animation(x, y, (U8*) dm_image->image, &(dm_image->image_handle));
            }
            else
            {
                x = dm_aligned_area_data.S16X + ((dm_aligned_area_data.S16width - image_width) >> 1);
                y = dm_aligned_area_data.S16Y;
                gdi_image_draw_animation(x, y, (U8*) dm_image->image, &(dm_image->image_handle));
            }
        }
        dm_image->x = x;
        dm_image->y = y;
        dm_image->width = image_width;
        dm_image->height = image_height;

        dm_aligned_area_data.S16Y += (S16) image_height;
        gui_pop_clip();
        return 1;
    }
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_button
 * DESCRIPTION
 *  This function is used setup the icon button based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  index                   [IN]            Index of icon button array
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_button(S16 **UICtrlAccessPtr_p, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates button_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &button_info);
    if (button_info.S16X == -1 || button_info.S16Y == -1 || button_info.S16Width == -1 || button_info.S16Height == -1)
    {
        return 0;
    }

    gdi_layer_push_clip();
    gdi_layer_set_clip(
        button_info.S16X,
        button_info.S16Y,
        button_info.S16X + button_info.S16Width - 1,
        button_info.S16Y + button_info.S16Height - 1);

    if (dm_button_array[index].x == -1 || dm_button_array[index].y == -1)
    {
        gui_move_icontext_button(&dm_button_array[index], button_info.S16X, button_info.S16Y);
    }

    if (dm_button_array[index].width == -1 || dm_button_array[index].height == -1)
    {
        gui_resize_icontext_button(&dm_button_array[index], button_info.S16Width, button_info.S16Height);
    }

    gui_show_icontext_button(&dm_button_array[index]);

    gdi_layer_pop_clip();

    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_back_fill_area
 * DESCRIPTION
 *  This function is used setup the back fill area set based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  index                   [IN]            
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_back_fill_area(S16 **UICtrlAccessPtr_p, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_back_fill_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_back_fill_info);
    if (dm_back_fill_info.S16X == -1 || dm_back_fill_info.S16Y == -1 ||
        dm_back_fill_info.S16Width == -1 || dm_back_fill_info.S16Height == -1)
    {
        return 0;
    }
    gui_push_clip();
    gui_set_clip(
        dm_back_fill_info.S16X,
        dm_back_fill_info.S16Y,
        dm_back_fill_info.S16X + dm_back_fill_info.S16Width - 1,
        dm_back_fill_info.S16Y + dm_back_fill_info.S16Height - 1);

    gui_draw_filled_area(
        dm_back_fill_info.S16X,
        dm_back_fill_info.S16Y,
        dm_back_fill_info.S16X + dm_back_fill_info.S16Width - 1,
        dm_back_fill_info.S16Y + dm_back_fill_info.S16Height - 1,
        dm_back_fill_array[index]);

    gui_pop_clip();

    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_rectangle
 * DESCRIPTION
 *  This function is used setup the rectangle based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  index                   [IN]            
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_rectangle(S16 **UICtrlAccessPtr_p, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_rectangle_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_rectangle_info);
    if (dm_rectangle_info.S16X == -1 || dm_rectangle_info.S16Y == -1 ||
        dm_rectangle_info.S16Width == -1 || dm_rectangle_info.S16Height == -1)
    {
        return 0;
    }
    gui_push_clip();
    gui_set_clip(
        dm_rectangle_info.S16X,
        dm_rectangle_info.S16Y,
        dm_rectangle_info.S16X + dm_rectangle_info.S16Width - 1,
        dm_rectangle_info.S16Y + dm_rectangle_info.S16Height - 1);
 
    if (dm_rectangle_array[index].fill_rectangle == DM_RECTANGLE_FILL_GRAYSCALE)
    {
        gui_greyscale_rectangle(
            dm_rectangle_info.S16X,
            dm_rectangle_info.S16Y,
            dm_rectangle_info.S16X + dm_rectangle_info.S16Width - 1,
            dm_rectangle_info.S16Y + dm_rectangle_info.S16Height - 1,
            MMI_BG_GREYSCALE_VALUE,
            MMI_BG_GREYSCALE_BLACK_VALUE);
    }
    else if (dm_rectangle_array[index].fill_rectangle == DM_RECTANGLE_FILL_COLOR)
  
    {
        gui_fill_rectangle(
            dm_rectangle_info.S16X,
            dm_rectangle_info.S16Y,
            dm_rectangle_info.S16X + dm_rectangle_info.S16Width - 1,
            dm_rectangle_info.S16Y + dm_rectangle_info.S16Height - 1,
            dm_rectangle_array[index].line_color);
    }
    else
    {
        gui_draw_rectangle(
            dm_rectangle_info.S16X,
            dm_rectangle_info.S16Y,
            dm_rectangle_info.S16X + dm_rectangle_info.S16Width - 1,
            dm_rectangle_info.S16Y + dm_rectangle_info.S16Height - 1,
            dm_rectangle_array[index].line_color);
    }

    gui_pop_clip();

    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_line
 * DESCRIPTION
 *  This function is used setup the line based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  index                   [IN]            
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_line(S16 **UICtrlAccessPtr_p, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_line_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_line_info);
    if (dm_line_info.S16X == -1 || dm_line_info.S16Y == -1 ||
        dm_line_info.S16Width == -1 || dm_line_info.S16Height == -1)
    {
        return 0;
    }
    gui_push_clip();
    gui_set_clip(
        dm_line_info.S16X,
        dm_line_info.S16Y,
        dm_line_info.S16X + dm_line_info.S16Width - 1,
        dm_line_info.S16Y + dm_line_info.S16Height - 1);

    gui_line(
        dm_line_info.S16X,
        dm_line_info.S16Y,
        dm_line_info.S16X + dm_line_info.S16Width - 1,
        dm_line_info.S16Y + dm_line_info.S16Height - 1,
        dm_line_array[index]);

    gui_pop_clip();
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_calendar
 * DESCRIPTION
 *  This function is used setup the calendat control based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_calendar(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_calendar_info_struct dm_calendar_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_calendar_coordinates(*UICtrlAccessPtr_p, &dm_calendar_info);
    if (dm_calendar_info.coordinate.S16X == -1 || dm_calendar_info.coordinate.S16Y == -1 ||
        dm_calendar_info.coordinate.S16Width == -1 || dm_calendar_info.coordinate.S16Height == -1)
    {
        return 0;
    }

    wgui_reset_calendar_params(
        dm_calendar_info.coordinate.S16X,
        dm_calendar_info.coordinate.S16Y,
        dm_calendar_info.calendar_columns,
        dm_calendar_info.calendar_rows,
        dm_calendar_info.coordinate.S16Width / dm_calendar_info.calendar_columns,
        dm_calendar_info.coordinate.S16Height / dm_calendar_info.calendar_rows);

    set_calendar_control_background_limits(
        dm_calendar_info.calendar_background_y,
        dm_calendar_info.calendar_background_y + dm_calendar_info.calendar_background_width);

    /* gui_set_font(&MMI_default_font); */
    cat82_title1_x1 = dm_calendar_info.title1_x;
    cat82_title1_y1 = dm_calendar_info.title1_y;
    cat82_title1_x2 = dm_calendar_info.title1_x + dm_calendar_info.title1_width - 1;
    cat82_title1_y2 = dm_calendar_info.title1_y + dm_calendar_info.title1_height - 1;
    calendar_title_font = dm_get_font(dm_calendar_info.title1_font);

     
#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    cat82_title1_month_x1 = dm_calendar_info.title1_month_x;
    cat82_title1_month_y1 = dm_calendar_info.title1_month_y;
    cat82_title1_month_x2 = dm_calendar_info.title1_month_x + dm_calendar_info.title1_month_width - 1;
    cat82_title1_month_y2 = dm_calendar_info.title1_month_y + dm_calendar_info.title1_month_height - 1;
    calendar_title_month_font = dm_get_font(dm_calendar_info.title1_month_font);
    calendar_show_title_month = dm_calendar_info.title1_month_show;
     
#ifdef __MMI_TOUCH_SCREEN__
    if (!calendar_show_title_month) /* Don't wanna show Month on title 1: Month so disable */
    {
        DisableCalendarTitle1MonthArrow();
    }
#endif /* __MMI_TOUCH_SCREEN__ */ 
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

    cat82_title2_x1 = dm_calendar_info.title2_x;
    cat82_title2_y1 = dm_calendar_info.title2_y;
    cat82_title2_x2 = dm_calendar_info.title2_x + dm_calendar_info.title2_width - 1;
    cat82_title2_y2 = dm_calendar_info.title2_y + dm_calendar_info.title2_height - 1;
    calendar_show_title2 = dm_calendar_info.title2_show;

#if defined(__MMI_UI_CALENDAR_WITH_INFO_BOX__) && defined (__MMI_TOUCH_SCREEN__)
    if (lunar_calendar_present)
    {
        if (!calendar_show_title2)  /* Don't wanna show Year on title 2 so disable */
        {
            DisableCalendarTitle1Arrow();
        }
    }
#endif /* defined(__MMI_UI_CALENDAR_WITH_INFO_BOX__) && defined (__MMI_TOUCH_SCREEN__) */ 

    cat82_title3_x1 = dm_calendar_info.title3_x;
    cat82_title3_y1 = dm_calendar_info.title3_y;
    cat82_title3_x2 = dm_calendar_info.title3_x + dm_calendar_info.title3_width - 1;
    cat82_title3_y2 = dm_calendar_info.title3_y + dm_calendar_info.title3_height - 1;
    calendar_show_title3 = dm_calendar_info.title3_show;

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    wgui_set_calendar_info_box(
        dm_calendar_info.infobox_x,
        dm_calendar_info.infobox_y,
        dm_calendar_info.infobox_width,
        dm_calendar_info.infobox_height,
        dm_calendar_info.infobox_max,
        dm_calendar_info.infobox_show);
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

    wgui_show_calendar();
    ChangeCategory82Title1(Cat82_title1);

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    ChangeCategory82Title1Month(Cat82_title1_month);
#endif 
#if !defined(__MMI_MAINLCD_160X128__)
    ChangeCategory82Title2(Cat82_title2);
#endif
    ChangeCategory82Title3(Cat82_title3);

    return 1;
}

/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_popup_background
 * DESCRIPTION
 *  This function is used setup the popup background based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_popup_background(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_popup_background_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_popup_background_info);
    if (dm_popup_background_info.S16X == -1 || dm_popup_background_info.S16Y == -1 ||
        dm_popup_background_info.S16Width == -1 || dm_popup_background_info.S16Height == -1)
    {
        return 0;
    }
    if ((g_dm_data.S32flags & DM_NO_POPUP_BACKGROUND))
    {
        return 0;
    }

#ifdef __MMI_NOKIA_STYLE_N800__
    if (!(SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW == g_dm_data.S32ScrId 
		|| SCR_ID_PHNSET_POWER_OFF_SYSTEM_VIEW == g_dm_data.S32ScrId
		|| SCR_WALLPAPER_VIEW == g_dm_data.S32ScrId
		|| SCR_SCREENSAVER_VIEW == g_dm_data.S32ScrId
	//	||   MMI_CATEGORY105_ID == g_dm_data.S32CatId
		|| ((POPUP_SCREENID == g_dm_data.S32ScrId
		        ||   KPAD_SCR_UNLOCK_HELP_MESSAGE == g_dm_data.S32ScrId
		        ||   KPAD_SCR_UNLOCK_MESSAGE == g_dm_data.S32ScrId 
		        ||   MMI_CATEGORY64_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_NONE_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_STATUS_ICON_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_NORMAL_ID == g_dm_data.S32CatId
		      )  &&( IDLE_SCREEN_ID == GetCurrScrnId()))
          )
     )
     {
            if (dm_popup_background_info.Flags & DM_POPUP_BACKGROUND_HATCH_FILL)
            {
//                color_t c = {200, 200, 200, 100};
              //  UI_cross_hatch_fill_rectangle(0, 0, UI_device_width - 1, UI_device_height - 1, c);
                entry_full_screen();
                g_dm_cntx.exit_full_screen_on_exit = MMI_TRUE;
            }
            else if (dm_popup_background_info.Flags & DM_POPUP_BACKGROUND_GREYSCALE)      
            {
//                gui_greyscale_rectangle(0, 0, UI_device_width - 1, UI_device_height - 1, MMI_BG_GREYSCALE_VALUE, MMI_BG_GREYSCALE_BLACK_VALUE);      
            }
     }
#endif
    if (dm_popup_background_info.Flags & DM_DRAW_POPUP_BACKGROUND_3D)
    {
    	#if defined(__MMI_MAINLCD_160X128__)
    	color_t c = {110, 110, 110, 100};
	wgui_pop_up_dialog_background.c = c;	
	#endif
        wgui_draw_pop_up_screen_background(
            dm_popup_background_info.S16X,
            dm_popup_background_info.S16Y,
            dm_popup_background_info.S16X + dm_popup_background_info.S16Width - 1,
            dm_popup_background_info.S16Y + dm_popup_background_info.S16Height - 1,
            &wgui_pop_up_dialog_background,
            MMI_TRUE);
    }
    else
    {
        wgui_draw_pop_up_screen_background(
            dm_popup_background_info.S16X,
            dm_popup_background_info.S16Y,
            dm_popup_background_info.S16X + dm_popup_background_info.S16Width - 1,
            dm_popup_background_info.S16Y + dm_popup_background_info.S16Height - 1,
            &wgui_pop_up_dialog_background,
            MMI_FALSE);
    }
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_scroll_text
 * DESCRIPTION
 *  This function is used setup the popup background based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  index                   [IN]            
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_scroll_text(S16 **UICtrlAccessPtr_p, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_scroll_text_info;
    S32 width = 0, height = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_scroll_text_info);
    if (dm_scroll_text_info.S16X == -1 || dm_scroll_text_info.S16Y == -1 ||
        dm_scroll_text_info.S16Width == -1 || dm_scroll_text_info.S16Height == -1)
    {
        return 0;
    }

    if (dm_scroll_text_buf[index][0] == 0 && dm_scroll_text_buf[index][1] == 0)
    {
        return 0;
    }

    dm_current_scroll_text = &dm_scroll_text[index];
    if (dm_scroll_text_info.Flags & DM_SCROLL_TEXT_CENTRE_ALIGN_X)
        if (dm_current_scroll_text->text_width >= dm_scroll_text_info.S16Width)
        {
            dm_current_scroll_text->x = (S32) dm_scroll_text_info.S16X;
        }
        else
        {
            dm_current_scroll_text->x =
                (S32) dm_scroll_text_info.S16X + ((dm_scroll_text_info.S16Width - dm_current_scroll_text->width) >> 1);
        }
    else
    {
        dm_current_scroll_text->x = (S32) dm_scroll_text_info.S16X;
    }

    if (dm_scroll_text_info.Flags & DM_SCROLL_TEXT_CENTER_ALIGN_Y)
    {
        dm_current_scroll_text->y =
            (S32) (dm_scroll_text_info.S16Y + ((dm_scroll_text_info.S16Height - dm_current_scroll_text->height) >> 1));
    }
    else
    {
        dm_current_scroll_text->y = (S32) dm_scroll_text_info.S16Y;
    }

    dm_current_scroll_text->width = (S32) dm_scroll_text_info.S16Width;
    if (dm_scroll_text_info.Flags & DM_SCROLL_TEXT_USE_FONT_HEIGHT)
    {
        gui_measure_string(dm_current_scroll_text->text, &width, &height);
        dm_current_scroll_text->height = height;
        dm_scroll_text_info.S16Height = height;
    }
    else
    {
        dm_current_scroll_text->height = (S32) dm_scroll_text_info.S16Height;
    }
    setup_previous_control(&dm_scroll_text_info);
    gui_show_scrolling_text(dm_current_scroll_text);
    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_aligned_area
 * DESCRIPTION
 *  This function is used setup the aligned area based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  control_set_ptr         [?]             
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_aligned_area(S16 **UICtrlAccessPtr_p, U8 *control_set_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_aligned_area_info;
    U8 i = 1, number_of_elements = 0;
    S32 total_element_height = 0, element_height = 0, element_width = 0;
    S32 image_count = 0, string_count = 0;
    S32 is_multiline = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Get aligned area range and flags */
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_aligned_area_info);
    dm_aligned_area_data.S16X = dm_aligned_area_info.S16X;
    dm_aligned_area_data.S16Y = dm_aligned_area_info.S16Y;
    dm_aligned_area_data.Flags = dm_aligned_area_info.Flags;
    dm_aligned_area_data.S16width = dm_aligned_area_info.S16Width;
    dm_aligned_area_data.S16height = dm_aligned_area_info.S16Height;
    if (!(dm_aligned_area_data.Flags & DM_ALLIGNED_AREA_NO_BACK_FILL))
    {
        gui_draw_filled_area(
            dm_aligned_area_info.S16X,
            dm_aligned_area_info.S16Y,
            dm_aligned_area_info.S16X + dm_aligned_area_info.S16Width,
            dm_aligned_area_info.S16Y + dm_aligned_area_info.S16Height,
            current_MMI_theme->general_background_filler);
    }
    /* Get Number of elements in aligned area */
    while (control_set_ptr[i - 1] != DM_ALIGNED_AREA_START) /* find the start of aligned area */
    {
        i++;
        if (control_set_ptr[i - 1] == DM_IMAGE)
        {
            image_count++;  /* skip those images not in aligned area. */
        }
        if (control_set_ptr[i - 1] == DM_STRING)
        {
            string_count++; /* skip those strings not in aligned area. */
        }
        MMI_ASSERT(i < 20); /* avoid infinite while loop. */
    }
    /* Assume all is vertically arranged, horizontally center alignment */
    while (control_set_ptr[i] != DM_ALIGNED_AREA_END)   /* start to calculate number of items and its' height */
    {
        MMI_ASSERT(i < 20); /* avoid infinite while loop. */
        switch (control_set_ptr[i])
        {
            case DM_SINGLELINE_INPUTBOX1:
                if (get_singleline_inputbox_display_status())
                {
                    resize_singleline_inputbox(dm_aligned_area_info.S16Width, MMI_singleline_inputbox_height);
                    number_of_elements++;
                    element_width = dm_aligned_area_info.S16Width;
                    element_height = MMI_singleline_inputbox_height;
                }
                else
                {
                    element_width = 0;
                    element_height = 0;
                }
                break;
            case DM_MULTILINE_INPUTBOX1:
                is_multiline = 1;
                if (get_multiline_inputbox_display_status())
                {
                    resize_multiline_inputbox_fit(
                        dm_aligned_area_info.S16Width,
                        dm_aligned_area_info.S16Height,
                        &element_width,
                        &element_height);
                    number_of_elements++;
                }
                else
                {
                    element_width = 0;
                    element_height = 0;
                }
                break;
            case DM_IMAGE:
                if (dm_image_array[image_count].image != NULL)  /* from resource */
                {
                    gdi_image_get_dimension(dm_image_array[image_count++].image, &element_width, &element_height);
                    number_of_elements++;
                }
                else if (dm_image_array[image_count].name != NULL)      /* from file */
                {
                    gdi_image_get_dimension_file(
                        (S8*) dm_image_array[image_count++].name,
                        &element_width,
                        &element_height);
                    number_of_elements++;
                }
                else    /* empty image. */
                {
                    element_width = 0;
                    element_height = 0;
                    image_count++;
                }
                #ifdef __MMI_NOKIA_STYLE_N800__
                 if(element_height )
                    element_height=element_height+4;
                #endif
                break;
            case DM_STRING:
                if (dm_string_array[string_count].string != NULL)
                {
                    gui_measure_string(dm_string_array[string_count++].string, &element_width, &element_height);
                    number_of_elements++;
                }
                else
                {
                    element_width = 0;
                    element_height = 0;
                    string_count++;
                }
                break;
            case DM_SLIDE_CONTROL:
                break;
            case DM_MATRIX_MENU1:
                break;
            default:
                MMI_ASSERT(0);  /* support above controls ONLY */
        }
        total_element_height += element_height;
        i++;
    }
    if (dm_aligned_area_data.Flags & DM_ALLIGNED_AREA_EQUAL_SPACE_TOP_AND_BOTTOM)
    {
        if (total_element_height < dm_aligned_area_info.S16Height)
        {
            dm_aligned_area_data.S16Y += ((dm_aligned_area_info.S16Height - total_element_height) >> 1);
            dm_aligned_area_data.S16height -= ((dm_aligned_area_info.S16Height - total_element_height) >> 1);
            dm_aligned_area_data.y_space = 0;
        }
        else
        {
            if (is_multiline && dm_aligned_area_data.Flags & DM_ALLIGNED_AREA_MULTILINE_SCROLL_IF_REQUIRED)
            {
                S32 h;

                MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR;
                h = dm_aligned_area_info.S16Height - (total_element_height - MMI_multiline_inputbox.height);
                /* 
                fh = get_multiline_inputbox_line_height();
                new_n_lines = (h - MULTILINE_INPUTBOX_HEIGHT_PAD - MMI_multiline_inputbox.text_y) / fh;
                h = (new_n_lines * fh) + MULTILINE_INPUTBOX_HEIGHT_PAD + MMI_multiline_inputbox.text_y;
                */
                /* h-1 avoid overlap */
                resize_multiline_inputbox(MMI_multiline_inputbox.width, h - 1);
                MMI_multiline_inputbox.edit_width =
                    MMI_multiline_inputbox.width - MMI_multiline_inputbox.vbar.width - 2;
                register_keyboard_key_handler(category151_keyboard_handler);
                SetKeyHandler(multiline_inputbox_previous_line, KEY_UP_ARROW, KEY_EVENT_DOWN);
                SetKeyHandler(multiline_inputbox_next_line, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
            }
            else
            {
                S32 h = dm_aligned_area_info.S16Height - (total_element_height - MMI_multiline_inputbox.height);

                resize_multiline_inputbox(MMI_multiline_inputbox.width, h);
            }
        }
    }
    else
    {
        dm_aligned_area_data.y_space =
            (dm_aligned_area_info.S16Height - total_element_height) / (number_of_elements + 1);
    }
    dm_aligned_area_data.x_space = 0;
    return 1;
}   /* end of dm_setup_and_draw_aligned_area */


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_slide_control
 * DESCRIPTION
 *  This function is used setup the slide control based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  index                   [IN]            
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_slide_control(S16 **UICtrlAccessPtr_p, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_slide_control_info;
    slide_control *previous_current_slide_control = MMI_current_slide_control;
    S32 value_width = 0;
    S32 idx = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Get aligned area range and flags */
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_slide_control_info);
    if (dm_slide_control_info.S16X == -1 || dm_slide_control_info.S16Y == -1 ||
        dm_slide_control_info.S16Width == -1 || dm_slide_control_info.S16Height == -1)
    {
        return 0;
    }

    if (dm_slide_control_info.Flags & DM_SLIDE_CONTROL_VALUE_POS_NONE)
    {
        MMI_slide_control[index].flags = GUI_SLIDE_CONTROL_VALUE_POS_NONE;
    }
    else if (dm_slide_control_info.Flags & DM_SLIDE_CONTROL_VALUE_POS_HEAD)
    {
        /*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-4-28 17:27 */
        //MMI_slide_control[index].flags = GUI_SLIDE_CONTROL_VALUE_POS_HEAD;
        MMI_slide_control[index].flags = GUI_SLIDE_CONTROL_VALUE_POS_NEXT_LINE;//GUI_SLIDE_CONTROL_VALUE_POS_HEAD;
        /*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-4-28 17:27 */
    }
    else if (dm_slide_control_info.Flags & DM_SLIDE_CONTROL_VALUE_POS_TAIL)
    {
        MMI_slide_control[index].flags = GUI_SLIDE_CONTROL_VALUE_POS_TAIL;
    }
    else if (dm_slide_control_info.Flags & DM_SLIDE_CONTROL_VALUE_POS_NEXT_LINE)
    {
        MMI_slide_control[index].flags = GUI_SLIDE_CONTROL_VALUE_POS_NEXT_LINE;
    }

    if (g_dm_cntx.redraw_screen_main != REDRAW_2_REDRAW)
    {
        for (idx = 0; idx < dm_slide_control_count; idx++)
        {
            if (MMI_slide_control[idx].value_string_width > value_width)
            {
                value_width = MMI_slide_control[idx].value_string_width;
            }
        }

        /* if ( dm_slide_control_info.Flags&DM_CENTRE_ALIGN_X) */
        switch (MMI_slide_control[index].flags)
        {
            case GUI_SLIDE_CONTROL_VALUE_POS_HEAD:
                dm_slide_control_info.S16X +=
                    ((dm_slide_control_info.S16Width - MMI_slide_control[index].bg_image_width - value_width) >> 1) +
                    value_width;
                MMI_slide_control[index].value_string_gap =
                    (value_width - MMI_slide_control[index].value_string_width) >> 1;
                break;

            case GUI_SLIDE_CONTROL_VALUE_POS_TAIL:
                dm_slide_control_info.S16X +=
                    (dm_slide_control_info.S16Width - MMI_slide_control[index].bg_image_width - value_width) >> 1;
                MMI_slide_control[index].value_string_gap =
                    (value_width - MMI_slide_control[index].value_string_width) >> 1;
                break;

            case GUI_SLIDE_CONTROL_VALUE_POS_NEXT_LINE:
                dm_slide_control_info.S16X +=
                    (dm_slide_control_info.S16Width - MMI_slide_control[index].bg_image_width) >> 1;
                MMI_slide_control[index].value_string_gap = 0;
                break;

            case GUI_SLIDE_CONTROL_VALUE_POS_NONE:
                break;

            default:
                break;
        }
        /* if ( MMI_slide_control[index].flags == GUI_SLIDE_CONTROL_VALUE_POS_TAIL ) */

        dm_slide_control_info.S16Height = MMI_slide_control[index].bg_image_height;
        setup_previous_control(&dm_slide_control_info);
        move_slide_control_value(&MMI_slide_control[index], dm_slide_control_info.S16X, dm_slide_control_info.S16Y);
    }

    MMI_current_slide_control = &MMI_slide_control[index];
    wgui_show_slide_control();
    if (MMI_current_slide_control == previous_current_slide_control)
    {
        wgui_show_slide_control_focus();
    }
    MMI_current_slide_control = previous_current_slide_control;

    return 1;
}   /* end of dm_setup_and_draw_slide_control */


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_typical_stopwatch
 * DESCRIPTION
 *  This function is used setup the typical stopwatch based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_typical_stopwatch(S16 **UICtrlAccessPtr_p)
{
#ifdef __MMI_STOPWATCH__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_typical_stopwatch_info_struct dm_typical_stopwatch_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_typical_stopwatch_coordinates(*UICtrlAccessPtr_p, &dm_typical_stopwatch_info);
    if (dm_typical_stopwatch_info.coordinate.S16X == -1 || dm_typical_stopwatch_info.coordinate.S16Y == -1 ||
        dm_typical_stopwatch_info.coordinate.S16Width == -1 || dm_typical_stopwatch_info.coordinate.S16Height == -1)
    {
        return 0;
    }

    wtch_pos.typ_timer.x = dm_typical_stopwatch_info.typ_timer_x;
    wtch_pos.typ_timer.y = dm_typical_stopwatch_info.typ_timer_y;
    wtch_pos.typ_milsec.x = dm_typical_stopwatch_info.typ_milsec_x;
    wtch_pos.typ_milsec.y = dm_typical_stopwatch_info.typ_milsec_y;
    wtch_pos.typ_history_height = dm_typical_stopwatch_info.typ_history_height;
    move_fixed_list(dm_typical_stopwatch_info.coordinate.S16X, dm_typical_stopwatch_info.coordinate.S16Y);
    resize_fixed_list(dm_typical_stopwatch_info.coordinate.S16Width, dm_typical_stopwatch_info.typ_history_height);
    stop_watch_x = (dm_typical_stopwatch_info.coordinate.S16Width >> 1) - (stop_watch_width >> 1);
    stop_watch_y = dm_typical_stopwatch_info.coordinate.S16Y + MMI_fixed_list_menu.height;
    show_typical_stop_watch();
    return 1;
#else /* __MMI_STOPWATCH__ */ 
    return 0;
#endif /* __MMI_STOPWATCH__ */ 
}   /* end of dm_setup_and_draw_typical_stopwatch */


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_nway_stopwatch
 * DESCRIPTION
 *  This function is used setup the N Way stopwatch based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_nway_stopwatch(S16 **UICtrlAccessPtr_p)
{
#ifdef __MMI_STOPWATCH__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_nway_stopwatch_info_struct dm_nway_stopwatch_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_nway_stopwatch_coordinates(*UICtrlAccessPtr_p, &dm_nway_stopwatch_info);
    if (dm_nway_stopwatch_info.coordinate.S16X == -1 || dm_nway_stopwatch_info.coordinate.S16Y == -1 ||
        dm_nway_stopwatch_info.coordinate.S16Width == -1 || dm_nway_stopwatch_info.coordinate.S16Height == -1)
    {
        return 0;
    }

    wtch_pos.nWay_maintimer_start.x = dm_nway_stopwatch_info.nway_main_timer_x1;
    wtch_pos.nWay_maintimer_start.y = dm_nway_stopwatch_info.nway_main_timer_y1;
    wtch_pos.nWay_maintimer_end.x = dm_nway_stopwatch_info.nway_main_timer_x2;
    wtch_pos.nWay_maintimer_end.y = dm_nway_stopwatch_info.nway_main_timer_y2;

    wtch_pos.nWay_dialog_start.x = dm_nway_stopwatch_info.nway_dialog_x1;
    wtch_pos.nWay_dialog_start.y = dm_nway_stopwatch_info.nway_dialog_y1;
    wtch_pos.nWay_dialog_end.x = dm_nway_stopwatch_info.nway_dialog_x2;
    wtch_pos.nWay_dialog_end.y = dm_nway_stopwatch_info.nway_dialog_y2;

    wtch_pos.separator_height = dm_nway_stopwatch_info.separator_height;

    show_nway_stop_watch();
    return 1;
#else /* __MMI_STOPWATCH__ */ 
    return 0;
#endif /* __MMI_STOPWATCH__ */ 
}   /* dm_get_nway_stopwatch_coordinates */


/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_wallpaper
 * DESCRIPTION
 *  This function is used setup wallpaper based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
S32 dm_setup_and_draw_wallpaper(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_wallpaper_info;
	mmi_trace(g_sw_WpSsPwon, "gdm:into dm_setup_and_draw_wallpaper()");

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_wallpaper_info);
    if (dm_wallpaper_info.S16X == -1 || dm_wallpaper_info.S16Y == -1 ||
        dm_wallpaper_info.S16Width == -1 || dm_wallpaper_info.S16Height == -1)
    {
        return 0;
    }

    if ((g_dm_cntx.redraw_screen_main == REDRAW_2_REDRAW) && (dm_wallpaper_layer != GDI_ERROR_HANDLE))
    {
        gdi_layer_push_and_set_active(dm_layers[dm_wallpaper_layer]);
        draw_wallpaper();
        gdi_layer_pop_and_restore_active();
    }
    else
    {
    #ifdef __GDI_MEMORY_PROFILE_2__
        gdi_layer_multi_layer_enable();
#if defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__)
if(g_dm_scr_bg_cntx.layer_handle == GDI_ERROR_HANDLE)
{
        gdi_layer_create(
            dm_wallpaper_info.S16X,
            dm_wallpaper_info.S16Y,
            dm_wallpaper_info.S16Width,
            dm_wallpaper_info.S16Height,
            &g_dm_scr_bg_cntx.layer_handle);
}
	dm_layers[dm_layers_count] = g_dm_scr_bg_cntx.layer_handle;
#else
gdi_layer_create(
            dm_wallpaper_info.S16X,
            dm_wallpaper_info.S16Y,
            dm_wallpaper_info.S16Width,
            dm_wallpaper_info.S16Height,
            &dm_layers[dm_layers_count]);
#endif
        gdi_layer_set_active(dm_layers[dm_layers_count++]);
        /* gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT); */
        gdi_layer_clear(gdi_layer_get_background());
        dm_wallpaper_layer = dm_layers_count - 1;
    #endif /* __GDI_MEMORY_PROFILE_2__ */ 
        draw_wallpaper();
    }

    return 1;

}   /* dm_get_nway_stopwatch_coordinates */

/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_volume_level
 * DESCRIPTION
 *  This function is used setup wallpaper based on the coordinate set and
 *  then draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
 extern S32 volume_level_UI;
S32 dm_setup_and_draw_volume_level(S16 **UICtrlAccessPtr_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   U16 single_volume_level_width = 0;
   U16 single_volume_level_height = 0;
   U16 single_volume_level_x = 0;
   U16 single_volume_level_y = 0;
   U16 space_temp = 0;
   U16 i = 0;
   gdi_color	rect_color;
   gdi_color	frame_color;
   S8 Unicode_volume_level[64];
   S32 width= 0;
   S32 height= 0;
   S8 volume_level[32];
   single_volume_level_x = MMI_CONTENT_X + MMI_CONTENT_WIDTH/8;
   single_volume_level_width = MMI_CONTENT_WIDTH/8*6/9;
   single_volume_level_height = MMI_CONTENT_HEIGHT/2/7;
   space_temp = (MMI_CONTENT_WIDTH/8*6 - single_volume_level_width*7)/6;
   frame_color = gdi_act_color_from_rgb(255, 0, 0,128);
for(i = 0; i< 7 ; i++ )
{
       single_volume_level_y = MMI_CONTENT_Y + MMI_CONTENT_HEIGHT/4*3  - single_volume_level_height*(i+1);
	if(i < volume_level_UI)
	{
		rect_color = gdi_act_color_from_rgb(255, 255, 125,255);
	}
	else
	{
		rect_color = gdi_act_color_from_rgb(125, 125, 125,125);
	}
   gdi_draw_frame_rect(single_volume_level_x + i*single_volume_level_width + i*space_temp,
   					    single_volume_level_y,
   					    single_volume_level_x + (i+1)*single_volume_level_width + i*space_temp,
   					    single_volume_level_y + single_volume_level_height*(i+1),
   					    rect_color,
   					    frame_color,
   					    5);
}
   sprintf((S8*) volume_level, "%d", volume_level_UI);
   AnsiiToUnicodeString((S8*) Unicode_volume_level, volume_level);
   gui_set_font(&wgui_dialer_box_f1);
   gui_set_text_color(gui_color(255, 15, 117));
   gui_measure_string((UI_string_type) Unicode_volume_level, &width, &height);
   
   gui_move_text_cursor((MMI_CONTENT_WIDTH - width)/2,MMI_CONTENT_Y+MMI_CONTENT_HEIGHT/4*3+5);
   gui_print_text((UI_string_type) Unicode_volume_level);
    return 1;

}  
/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_percentage_bar
 * DESCRIPTION
 *  
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN]        
 *  count                   [IN]        
 * RETURNS
 *  
 *****************************************************************************/
S32 dm_setup_and_draw_percentage_bar(S16 **UICtrlAccessPtr_p, S32 count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_coordinates dm_percentage_bar_info;
    dm_percentage_bar_struct *act_bar = &dm_percentage_bar_array[count];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *UICtrlAccessPtr_p = dm_get_coordinates(*UICtrlAccessPtr_p, &dm_percentage_bar_info);
    if (dm_percentage_bar_info.S16X == -1 || dm_percentage_bar_info.S16Y == -1 ||
        dm_percentage_bar_info.S16Width == -1 || dm_percentage_bar_info.S16Height == -1)
    {
        return 0;
    }

    act_bar->x = dm_percentage_bar_info.S16X;
    act_bar->y = dm_percentage_bar_info.S16Y;
    act_bar->width = dm_percentage_bar_info.S16Width;
    act_bar->height = dm_percentage_bar_info.S16Height;

    dm_redraw_percentage_bar(count);

    return 1;
}

#if defined(__MMI_SCREEN_ROTATE__) // && defined(__MMI_WALLPAPER_ON_BOTTOM__)
#include "wallpaperdefs.h"
#include "screensaverdefs.h"
#include "poweronoffdisplaydefs.h"
#endif

#if defined(__MMI_NOKIA_STYLE_N800__) || defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
#include "mainmenudef.h"
#include "wallpaperdefs.h"
#include "poweronoffdisplaydefs.h"
#include "screensaverdefs.h"
#include "idleappdef.h"
#include "quickaccessmenuitem_def.h"
#include "audioplayerdef.h"
#endif
#if defined(__MMI_BLACKBERRY_QWERTY__)
void dm_draw_v300_proj_scr_bg_image(void)
{
	S32 img_width, img_height;
	GDI_RESULT ret;
	U16 wallPaper;

	PhnsetGetCurWallPaper(&wallPaper);
	if(wallPaper)
		gdi_image_draw_id(0,0,wallPaper);
	else
	{
		if(idle_screen_wallpaper_name)
		{
			gdi_image_get_dimension_file(idle_screen_wallpaper_name, &img_width, &img_height);
			if (img_width > UI_device_width || img_height > UI_device_height)   /* the image is too large to fit the screen */
			{
				S32 resized_offset_x, resized_offset_y;
				S32 resized_width, resized_height;

				gdi_image_util_fit_bbox(
										UI_device_width,
										UI_device_height,
										img_width,
										img_height,
										&resized_offset_x,
										&resized_offset_y,
										&resized_width,
										&resized_height);
				ret = gdi_image_draw_resized_file(
										resized_offset_x,
										resized_offset_y,
										resized_width,
										resized_height,
										(S8*) idle_screen_wallpaper_name);
			}
			else
			{
				S32 offset_x = (UI_device_width - img_width) >> 1;
				S32 offset_y = (UI_device_height - img_height) >> 1;

				gdi_image_draw_file(offset_x, offset_y, idle_screen_wallpaper_name);
			}
		}
	}
}
#endif
/*****************************************************************************
 * FUNCTION
 *  dm_draw_scr_bg_image
 * DESCRIPTION
 *  This function is used to draw scr bg image
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_draw_scr_bg_image(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	U16 bg_image_id = g_dm_scr_bg_cntx.image_id;
    S32 img_width, img_height;
    GDI_RESULT ret;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_and_set_active(g_dm_scr_bg_cntx.layer_handle);

#ifdef __MMI_LITE_DISPLAY__
    /* 082906 scr_bg Start */
    if (!(g_dm_scr_bg_cntx.flags && DM_SCR_BG_FORCE_SHOW))
    {
        UI_UNUSED_PARAMETER(img_width);
        UI_UNUSED_PARAMETER(img_height);
        UI_UNUSED_PARAMETER(ret);
        gdi_draw_solid_rect(
            g_dm_scr_bg_cntx.offset_x,
            g_dm_scr_bg_cntx.offset_y,
            UI_device_width - 1,
            UI_device_height - 1,
            gdi_act_color_from_rgb(0xFF, current_MMI_theme->lite_disp_scr_bg_color->r, current_MMI_theme->lite_disp_scr_bg_color->g, current_MMI_theme->lite_disp_scr_bg_color->b));
    }
    else
#endif /* __MMI_LITE_DISPLAY__ */
//#else /* __MMI_LITE_DISPLAY__ */
    TRACE_GDI_FUNCTION();
    mmi_trace(g_sw_GDI, "g_dm_scr_bg_cntx.image_id=%d, x = %d; y= %d",g_dm_scr_bg_cntx.image_id,g_dm_scr_bg_cntx.offset_x,g_dm_scr_bg_cntx.offset_y);

#if defined(__MMI_USING_BLACK_THEME_BG__)
	if ((IDLE_SCREEN_ID != g_dm_data.S32ScrId)&&
		!(SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW == g_dm_data.S32ScrId ||
                    SCR_ID_PHNSET_POWER_OFF_SYSTEM_VIEW == g_dm_data.S32ScrId ||
                    SCR_WALLPAPER_VIEW == g_dm_data.S32ScrId ||
                    SCR_SCREENSAVER_VIEW == g_dm_data.S32ScrId||
                    SCR_KEY_TONE_VOLUME == g_dm_data.S32ScrId||
                    KPAD_SCR_UNLOCK_HELP_MESSAGE == g_dm_data.S32ScrId||
                    KPAD_SCR_UNLOCK_MESSAGE == g_dm_data.S32ScrId                    
                     ))
	{
		bg_image_id = IMG_MAIN_MENU_MATRIX_BG;
	}
#endif

#if defined(__MMI_SCREEN_ROTATE__) // && defined(__MMI_WALLPAPER_ON_BOTTOM__)
		if(mmi_frm_is_screen_width_height_swapped()
                    && (!(SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW == g_dm_data.S32ScrId ||
                    SCR_ID_PHNSET_POWER_OFF_SYSTEM_VIEW == g_dm_data.S32ScrId ||
                    SCR_WALLPAPER_VIEW == g_dm_data.S32ScrId ||
                    SCR_SCREENSAVER_VIEW == g_dm_data.S32ScrId 
                     ))
		)
		{
			bg_image_id = IMG_SCREEN_ROTATE_BACKGROUND;
		}
        else
#endif	
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_NOKIA_STYLE_N800__)||(defined __MMI_MAINLCD_220X176__) ||(defined __MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_160X128__) || defined(__MMI_MAINLCD_320X240__) 
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#if defined (__MMI_MAINLCD_160X128__)
    if (!(SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW == g_dm_data.S32ScrId 
		|| SCR_ID_PHNSET_POWER_OFF_SYSTEM_VIEW == g_dm_data.S32ScrId
		|| SCR_WALLPAPER_VIEW == g_dm_data.S32ScrId
		|| SCR_SCREENSAVER_VIEW == g_dm_data.S32ScrId
		||   MMI_CATEGORY105_ID == g_dm_data.S32CatId
		|| POPUP_SCREENID == g_dm_data.S32ScrId
		|| MAIN_MENU_SCREENID == g_dm_data.S32ScrId
		        ||   KPAD_SCR_UNLOCK_HELP_MESSAGE == g_dm_data.S32ScrId
		        ||   KPAD_SCR_UNLOCK_MESSAGE == g_dm_data.S32ScrId 
		        ||   MMI_CATEGORY64_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_NONE_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_STATUS_ICON_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_NORMAL_ID == g_dm_data.S32CatId
		|| IDLE_SCREEN_ID == GetCurrScrnId()
		|| SCR_ID_AUDPLY_MAIN == GetCurrScrnId()
		#else
    if (!(SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW == g_dm_data.S32ScrId 
		|| SCR_ID_PHNSET_POWER_OFF_SYSTEM_VIEW == g_dm_data.S32ScrId
		|| SCR_WALLPAPER_VIEW == g_dm_data.S32ScrId
		|| SCR_SCREENSAVER_VIEW == g_dm_data.S32ScrId
		||   MMI_CATEGORY105_ID == g_dm_data.S32CatId
		|| ((POPUP_SCREENID == g_dm_data.S32ScrId
		        ||   KPAD_SCR_UNLOCK_HELP_MESSAGE == g_dm_data.S32ScrId
		        ||   KPAD_SCR_UNLOCK_MESSAGE == g_dm_data.S32ScrId 
		        ||   MMI_CATEGORY64_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_NONE_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_STATUS_ICON_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_NORMAL_ID == g_dm_data.S32CatId
		      )  &&( IDLE_SCREEN_ID == GetCurrScrnId()))
        #endif
          )
     )
    {
#if defined(__MMI_BLACKBERRY_QWERTY__)
	dm_draw_v300_proj_scr_bg_image();
#else
#if defined(__PROJECT_GALLITE_C01__)
       if (MMI_CATEGORY658_ID == g_dm_data.S32CatId)
       {
		gdi_image_draw_id(0,0,IMG_DIALING_KEY_BG);
       }
	else
#endif
	gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, current_MMI_theme->main_menu_background_filler);
#endif
        gdi_layer_pop_and_restore_active();

        return ;
    }
#endif

#if defined(__PROJECT_GALLITE_C01__) 
if((g_dm_data.S32ScrId == KPAD_SCR_UNLOCK_HELP_MESSAGE || KPAD_SCR_UNLOCK_MESSAGE == g_dm_data.S32ScrId)
   && mmi_msg_need_new_msg_ind())
{
	gui_draw_filled_area(0, 0, UI_device_width - 1, UI_device_height - 1, current_MMI_theme->main_menu_background_filler);
	gdi_layer_pop_and_restore_active();
	return;
}
#endif

    {
        if (bg_image_id != 0) /* from resource */
        {
            if (g_dm_scr_bg_cntx.offset_x == -1 && g_dm_scr_bg_cntx.offset_y == -1)
            {
                gdi_image_get_dimension_id(bg_image_id, &img_width, &img_height);
            	  mmi_trace(g_sw_GDI, "g_dm_scr_bg_cntx.image_id=%d, img_width = %d; img_height = %d",bg_image_id,img_width,img_height);

                if (img_width > UI_device_width || img_height > UI_device_height)   /* the image is too large to fit the screen */
                {
                    S32 resized_offset_x, resized_offset_y;
                    S32 resized_width, resized_height;

                    gdi_image_util_fit_bbox(
                        UI_device_width,
                        UI_device_height,
                        img_width,
                        img_height,
                        &resized_offset_x,
                        &resized_offset_y,
                        &resized_width,
                        &resized_height);

                    ret = gdi_image_draw_resized(
                            resized_offset_x,
                            resized_offset_y,
                            resized_width,
                            resized_height,
                            get_image(bg_image_id));
                }
                else
                {
                    S32 offset_x = (UI_device_width - img_width) >> 1;
                    S32 offset_y = (UI_device_height - img_height) >> 1;
#if defined(__MMI_BLACKBERRY_QWERTY__)
		if(MMI_CATEGORY105_ID == g_dm_data.S32CatId)
		{
			gdi_layer_lock_frame_buffer();		
			dm_draw_v300_proj_scr_bg_image();
			gdi_image_draw_id(offset_x, offset_y, bg_image_id);
			gdi_layer_unlock_frame_buffer();
		}
		else
#endif
#if defined(__PROJECT_GALLITE_C01__)
       extern S16 g_dm_pre_scr;
       if (IDLE_SCREEN_DIGIT_HANDLER_ID == g_dm_pre_scr && POPUP_SCREENID == g_dm_data.S32ScrId)
       {
		gdi_image_draw_id(0,0,IMG_DIALING_KEY_BG);
       }
	else
#endif
#if defined(__MMI_USING_BLACK_THEME_BG__)					
	                if (SCR_KEY_TONE_VOLUME == g_dm_data.S32ScrId)
	                {
	                	S32 img_w,img_h,off_x,off_y;
	                     gdi_image_get_dimension_id(IMG_MAIN_MENU_MATRIX_BG, &img_w, &img_h);
				off_x = (UI_device_width - img_w) >> 1;
				off_y = (UI_device_height - img_h) >> 1;		 
	                     gdi_image_draw_id(off_x, off_y, IMG_MAIN_MENU_MATRIX_BG);		
	                }                    
#endif
			gdi_image_draw_id(offset_x, offset_y, bg_image_id);
                }
            }
            else
            {
                gdi_image_draw_id(g_dm_scr_bg_cntx.offset_x, g_dm_scr_bg_cntx.offset_y, bg_image_id);
            }
        }
        else    /* from file */
        {
            if (g_dm_scr_bg_cntx.offset_x == -1 && g_dm_scr_bg_cntx.offset_y == -1)
            {
                if(gdi_image_get_dimension_file(g_dm_scr_bg_cntx.image_name, &img_width, &img_height) != 0)
                {
                    PhnsetRstWallpaperDefault();
                                        
                    return;
                }            

                if (img_width > UI_device_width || img_height > UI_device_height)   /* the image is too large to fit the screen */
                {
                    S32 resized_offset_x, resized_offset_y;
                    S32 resized_width, resized_height;

                    gdi_image_util_fit_bbox(
                        UI_device_width,
                        UI_device_height,
                        img_width,
                        img_height,
                        &resized_offset_x,
                        &resized_offset_y,
                        &resized_width,
                        &resized_height);
                    ret = gdi_image_draw_resized_file(
                            resized_offset_x,
                            resized_offset_y,
                            resized_width,
                            resized_height,
                            (S8*) g_dm_scr_bg_cntx.image_name);
                }
                else
                {
                    S32 offset_x = (UI_device_width - img_width) >> 1;
                    S32 offset_y = (UI_device_height - img_height) >> 1;

                    gdi_image_draw_file(offset_x, offset_y, g_dm_scr_bg_cntx.image_name);
                }
            }
            else
            {
                gdi_image_draw_file(g_dm_scr_bg_cntx.offset_x, g_dm_scr_bg_cntx.offset_y, g_dm_scr_bg_cntx.image_name);
            }
        }
    }
    /* 082906 scr_bg End */
    gdi_layer_pop_and_restore_active();
}


/*****************************************************************************
 * FUNCTION
 *  dm_set_scr_bg_image
 * DESCRIPTION
 *  This function is used to set image id or image name of scr bg image
 * PARAMETERS
 *  image_id        [IN]        Id of the image
 *  file_name       [IN]        File name of the image
 *  x               [IN]        
 *  y               [IN]        
 *  opacity         [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void dm_set_scr_bg_image(U16 image_id, S8 *file_name, S32 x, S32 y, U8 opacity)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

     
    if (image_id != 0 && file_name == NULL)
    {
        g_dm_scr_bg_cntx.image_id = image_id;
        g_dm_scr_bg_cntx.image_name = NULL;
    }
    else if (image_id == 0 && file_name != NULL)
    {
        g_dm_scr_bg_cntx.image_id = 0;
        g_dm_scr_bg_cntx.image_name = file_name;
    }
     
#ifndef MMI_ON_WIN32
    else
    {
        MMI_ASSERT(0);
    }
#endif /* MMI_ON_WIN32 */ 

#if defined(__MMI_PROJECT_GS_HK603__)
    if (MAIN_MENU_SCREENID == GetActiveScreenId())
    {
        g_dm_scr_bg_cntx.image_id = IMG_MAIN_MENU_BG2;
    }
    else      if (!(SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW == g_dm_data.S32ScrId 
		|| SCR_ID_PHNSET_POWER_OFF_SYSTEM_VIEW == g_dm_data.S32ScrId
		|| SCR_WALLPAPER_VIEW == g_dm_data.S32ScrId
		|| SCR_SCREENSAVER_VIEW == g_dm_data.S32ScrId
		||   MMI_CATEGORY105_ID == g_dm_data.S32CatId
		|| ((POPUP_SCREENID == g_dm_data.S32ScrId
		        ||   KPAD_SCR_UNLOCK_HELP_MESSAGE == g_dm_data.S32ScrId
		        ||   KPAD_SCR_UNLOCK_MESSAGE == g_dm_data.S32ScrId 
		        ||   MMI_CATEGORY64_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_NONE_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_STATUS_ICON_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_NORMAL_ID == g_dm_data.S32CatId
		      )  &&( IDLE_SCREEN_ID == GetCurrScrnId()))
          )
     )
    {
	g_dm_scr_bg_cntx.image_id = IMG_MAIN_MENU_BG1;
	g_dm_scr_bg_cntx.image_name = NULL;
    }
#endif
    g_dm_scr_bg_cntx.offset_x = x;
    g_dm_scr_bg_cntx.offset_y = y;
    g_dm_scr_bg_cntx.layer_opacity = opacity;    

     
    /* if g_dm_scr_bg_cntx.1scr_bg_layer does not exist */
    if (g_dm_scr_bg_cntx.layer_handle == GDI_ERROR_HANDLE)
    {
        return;
    }
     

    gdi_layer_push_and_set_active(g_dm_scr_bg_cntx.layer_handle);
    gdi_layer_set_opacity(TRUE, g_dm_scr_bg_cntx.layer_opacity);         
#ifdef __MMI_MAINLCD_220X176__    
    gdi_layer_clear(GDI_COLOR_BLACK);
#else
    gdi_layer_clear(GDI_COLOR_WHITE);
#endif
    dm_draw_scr_bg_image();
    gdi_layer_pop_and_restore_active();
}

 

#ifdef __T800_MAIN_MENU__
void dm_set_scr_bg_image_opacity(U8 opacity)
{
    g_dm_scr_bg_cntx.layer_opacity = opacity;   
}
#endif
/*****************************************************************************
 * FUNCTION
 *  dm_set_scr_bg_image_no_draw
 * DESCRIPTION
 *  This function is used to set image id or image name of scr bg image
 * PARAMETERS
 *  image_id        [IN]        Id of the image
 *  file_name       [IN]        File name of the image
 *  x               [IN]        
 *  y               [IN]        
 *  opacity         [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void dm_set_scr_bg_image_no_draw(U16 image_id, S8 *file_name, S32 x, S32 y, U8 opacity)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (image_id != 0 && file_name == NULL)
    {
        g_dm_scr_bg_cntx.image_id = image_id;
        g_dm_scr_bg_cntx.image_name = NULL;
    }
    else if (image_id == 0 && file_name != NULL)
    {
        g_dm_scr_bg_cntx.image_id = 0;
        g_dm_scr_bg_cntx.image_name = file_name;
    }
#ifndef MMI_ON_WIN32
    else
    {
        MMI_ASSERT(0);
    }
#endif /* MMI_ON_WIN32 */ 

#if defined(__MMI_PROJECT_GS_HK603__)
    if (MAIN_MENU_SCREENID == GetActiveScreenId())
    {
        g_dm_scr_bg_cntx.image_id = IMG_MAIN_MENU_BG2;
    }
    else     if (!(SCR_ID_PHNSET_POWER_ON_SYSTEM_VIEW == g_dm_data.S32ScrId 
		|| SCR_ID_PHNSET_POWER_OFF_SYSTEM_VIEW == g_dm_data.S32ScrId
		|| SCR_WALLPAPER_VIEW == g_dm_data.S32ScrId
		|| SCR_SCREENSAVER_VIEW == g_dm_data.S32ScrId
		||   MMI_CATEGORY105_ID == g_dm_data.S32CatId
		|| ((POPUP_SCREENID == g_dm_data.S32ScrId
		        ||   KPAD_SCR_UNLOCK_HELP_MESSAGE == g_dm_data.S32ScrId
		        ||   KPAD_SCR_UNLOCK_MESSAGE == g_dm_data.S32ScrId 
		        ||   MMI_CATEGORY64_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_NONE_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_STATUS_ICON_ID == g_dm_data.S32CatId
		        ||   MMI_CATEGORY66_NORMAL_ID == g_dm_data.S32CatId
		      )  &&( IDLE_SCREEN_ID == GetCurrScrnId()))
          )
     )
    {
	g_dm_scr_bg_cntx.image_id = IMG_MAIN_MENU_BG1;
	g_dm_scr_bg_cntx.image_name = NULL;
    }
#endif
    g_dm_scr_bg_cntx.offset_x = x;
    g_dm_scr_bg_cntx.offset_y = y;
    g_dm_scr_bg_cntx.layer_opacity = opacity;    
}

 


/*****************************************************************************
 * FUNCTION
 *  dm_get_scr_bg_image
 * DESCRIPTION
 *  This function is used to get image id or image name of scr bg image
 * PARAMETERS
 *  image_id        [IN/OUT]        Id of the image
 *  file_name       [IN/OUT]        File name of the image
 *  x               [?]             
 *  y               [?]             
 *  opacity         [?]             
 * RETURNS
 *  void
 *****************************************************************************/
 
void dm_get_scr_bg_image(U16 *image_id, S8 **file_name, S32 *x, S32 *y, U8 *opacity)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    if (g_dm_scr_bg_cntx.image_id == 0 && g_dm_scr_bg_cntx.image_name == NULL)
    {
        if (idle_screen_wallpaper_ID == 0)//073106 cache wallpaper
        {
            g_dm_scr_bg_cntx.image_name = idle_screen_wallpaper_name;
        }
        else
        {
           g_dm_scr_bg_cntx.image_id = idle_screen_wallpaper_ID;
        }
    }
     

    *image_id = g_dm_scr_bg_cntx.image_id;
    *file_name = g_dm_scr_bg_cntx.image_name;
    *x = g_dm_scr_bg_cntx.offset_x;
    *y = g_dm_scr_bg_cntx.offset_y;
    *opacity = g_dm_scr_bg_cntx.layer_opacity;
}


/*****************************************************************************
 * FUNCTION
 *  dm_set_scr_bg_redraw
 * DESCRIPTION
 *  An API for application to set the flag g_dm_cntx.redraw_scr_bg_flag
 * PARAMETERS
 *  status      [IN]        The new value of dm_draw_scr_bg_flag.
 * RETURNS
 *  void
 *****************************************************************************/
void dm_set_scr_bg_redraw(MMI_BOOL status)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_dm_cntx.redraw_scr_bg_flag == MMI_FALSE && status == MMI_TRUE)        /* should clear g_dm_scr_bg_cntx.1scr_bg_layer */
    {
        g_dm_cntx.redraw_scr_bg_flag = status;
    }
    else if (g_dm_cntx.redraw_scr_bg_flag == MMI_TRUE && status == MMI_FALSE)
    {
        /* SHOULD NOT HAPPEN */
        g_dm_cntx.redraw_scr_bg_flag = status;
    }
}


/*****************************************************************************
 * FUNCTION
 *  dm_set_scr_bg_opacity
 * DESCRIPTION
 *  This function is used to set opacity of scr_bg layer
 * PARAMETERS
 *  opacity     [IN]        Opacity of layer
 * RETURNS
 *  void
 *****************************************************************************/
void dm_set_scr_bg_opacity(U8 opacity)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_dm_scr_bg_cntx.layer_opacity = opacity;
}


/*****************************************************************************
 * FUNCTION
 *  dm_get_scr_bg_opacity
 * DESCRIPTION
 *  This function is used to get opacity of scr_bg layer
 * PARAMETERS
 *  opacity     [IN/OUT]        Opacity of layer
 * RETURNS
 *  void
 *****************************************************************************/
void dm_get_scr_bg_opacity(U8 *opacity)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *opacity = g_dm_scr_bg_cntx.layer_opacity;
}

/* 082906 scr_bg Start */
/*****************************************************************************
 * FUNCTION
 *  dm_set_scr_bg_flag
 * DESCRIPTION
 *  This function is used to set display flags of scr_bg layer
 * PARAMETERS
 *  flags     [IN]        display flags
 * RETURNS
 *  void
 *****************************************************************************/
void dm_set_scr_bg_flag(S32 flags)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_dm_scr_bg_cntx.flags = flags;
}


/*****************************************************************************
 * FUNCTION
 *  dm_get_scr_bg_flag
 * DESCRIPTION
 *  This function is used to get display flags of scr_bg layer
 * PARAMETERS
 *  flags     [IN/OUT]        display flags
 * RETURNS
 *  void
 *****************************************************************************/
void dm_get_scr_bg_flag(S32 *flags)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *flags = g_dm_scr_bg_cntx.flags;
}
/* 082906 scr_bg End */


/*****************************************************************************
 * FUNCTION
 *  dm_get_scr_bg_layer
 * DESCRIPTION
 *  This function is used to get the layer handle of scr bg image
 * PARAMETERS
 *  void
 * RETURNS
 *  GDI_HANDLE
 *****************************************************************************/
GDI_HANDLE dm_get_scr_bg_layer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return g_dm_scr_bg_cntx.layer_handle;
}

#if defined(__MMI_MAINLCD_128X128__)
#include "..\..\mainmenu\include\mainmenudef.h"
GDI_HANDLE fixed_list_menu_icon_layer_handle;
#endif
/*****************************************************************************
 * FUNCTION
 *  dm_setup_and_draw_scr_bg
 * DESCRIPTION
 *  This function is used setup screen background layer and draw it
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
#if defined(MEM_ULC_3216)
#include "wallpaperdefs.h"
#endif
#if defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__)
U16 dm_old_background_id;
#endif
S32 dm_setup_and_draw_scr_bg(S16 **UICtrlAccessPtr_p)
{
    /* Currently coordinate set is not needed. The input UICtrlAccessPtr_p is just for future use. */
#ifdef __GDI_MEMORY_PROFILE_2__

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    GDI_RESULT ret;
    U8 layer_idx = 0;
    TRACE_GDI_FUNCTION();
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_dm_cntx.redraw_scr_bg_flag == MMI_TRUE)
    {
        /* There is no free layer can be used for background */
        MMI_ASSERT(dm_layers_count < GDI_LAYER_TOTAL_LAYER_COUNT);

        gdi_layer_multi_layer_enable();
if(g_dm_scr_bg_cntx.layer_handle == GDI_ERROR_HANDLE)
{
        ret = gdi_layer_create(0, 0, UI_device_width, UI_device_height, &g_dm_scr_bg_cntx.layer_handle);

        /* Create layer fail */
        /* MMI_ASSERT (ret>0); */
        if (ret < 0)
        {
            return 0;
        }
}
#if defined(__MMI_MAINLCD_128X128__)
        if (MAIN_MENU_SCREENID == GetActiveScreenId())
        {
            fixed_list_menu_icon_layer_handle = dm_get_scr_bg_layer();
            dm_set_scr_bg_opacity(255);
        }
#endif
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_MAINLCD_320X240__) || defined(__PROJECT_GALLITE_C01__)
        if (MAIN_MENU_SCREENID == GetActiveScreenId())
        {
            pda_layer_handle = dm_get_scr_bg_layer();
            dm_set_scr_bg_opacity(255);
        }
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */
        gdi_layer_push_and_set_active(g_dm_scr_bg_cntx.layer_handle);
    #ifdef __MMI_SCREEN_ROTATE__
        /* gdi_layer_set_rotate(gdi_layer_get_base_layer_rotation()); */
    #endif 


        if (g_dm_scr_bg_cntx.image_id == 0 && g_dm_scr_bg_cntx.image_name == NULL)
        {
            if (idle_screen_wallpaper_ID == 0)//073106 cache wallpaper
            {            
#if (defined(MEM_ULC_3216) && !defined(__MMI_MAINLCD_160X128__))||defined(__MMI_PROJECT_GS_HK603__)
#ifdef __MMI_USE_DRAW_FOCUS_IN_MAINMENU__
if (MAIN_MENU_SCREENID == GetActiveScreenId())
        {
             g_dm_scr_bg_cntx.image_id = IMG_MAIN_MENU_BG2;
        }
else
#endif
        {
                g_dm_scr_bg_cntx.image_id = WALLPAPER_1;
        }
#else            
                g_dm_scr_bg_cntx.image_name = idle_screen_wallpaper_name;
#endif
            }
            else
            {
 #if (defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__))||defined(__MMI_PROJECT_GS_HK603__)
	#ifdef __MMI_USE_DRAW_FOCUS_IN_MAINMENU__
		if (MAIN_MENU_SCREENID == GetActiveScreenId())
		        {
		             g_dm_scr_bg_cntx.image_id = IMG_MAIN_MENU_BG2;
		        }
		else
	#endif
		{
		                g_dm_scr_bg_cntx.image_id = IMG_MAIN_MENU_BG1;
		}
#elif defined(MEM_ULC_3216) && !defined(__MMI_MAINLCD_160X128__)
#ifdef __MMI_USE_DRAW_FOCUS_IN_MAINMENU__
if (MAIN_MENU_SCREENID == GetActiveScreenId())
        {
             g_dm_scr_bg_cntx.image_id = IMG_MAIN_MENU_BG2;
        }
else
#endif
{
                g_dm_scr_bg_cntx.image_id = WALLPAPER_1;
}
#else
                g_dm_scr_bg_cntx.image_id = idle_screen_wallpaper_ID;
#endif		
            }
        }
#if defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__)
if((dm_old_background_id != g_dm_scr_bg_cntx.image_id)||(WALLPAPER_1 != g_dm_scr_bg_cntx.image_id))
#endif
{
	 gdi_layer_set_source_key(TRUE, GDI_COLOR_TRANSPARENT);
        gdi_layer_set_opacity(TRUE, g_dm_scr_bg_cntx.layer_opacity);
 //       gdi_layer_set_opacity(TRUE, 255);
        gdi_layer_clear(GDI_COLOR_TRANSPARENT);
        /* Start to draw the background */
        dm_draw_scr_bg_image();

        /* End of drawing the background */
}

        gdi_layer_pop_and_restore_active();

        /* set scr_bg layer to be the most buttom layer, and shift other layers */
        for (layer_idx = 0; layer_idx < dm_layers_count; layer_idx++)
        {
            dm_layers[layer_idx + 1] = dm_layers[layer_idx];
        }
        dm_layers[0] = g_dm_scr_bg_cntx.layer_handle;
        dm_layers_count++;

        /* Once scr_bg is drawn, there is no need to redraw again, except the flag is turned on */
        g_dm_cntx.redraw_scr_bg_flag = MMI_FALSE;

    }
#if defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__)
dm_old_background_id = g_dm_scr_bg_cntx.image_id;
#endif
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    wgui_set_wallpaper_on_bottom(MMI_TRUE);
#endif 
    return 1;

#else /* __GDI_MEMORY_PROFILE_2__ */ 
    return 0;

#endif /* __GDI_MEMORY_PROFILE_2__ */ 

}   /* dm_setup_and_draw_scr_bg */


/*****************************************************************************
 * FUNCTION
 *  dm_register_vkpad_callback
 * DESCRIPTION
 *  This function is to register dm_show_vkpad_callback
 * PARAMETERS
 *  f       [IN]        Function pointer to do changes before show virtual keypad.
 * RETURNS
 *  void
 *****************************************************************************/
void dm_register_vkpad_callback(FuncPtr f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_show_vkpad_callback = f;
}


/*****************************************************************************
 * FUNCTION
 *  dm_check_control_exist
 * DESCRIPTION
 *  This function is to register dm_show_vkpad_callback
 * PARAMETERS
 *  category_id     [IN]        
 *  control_id      [IN]        
 *  f(?)            [IN]        Function pointer to do changes before show virtual keypad.
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL dm_check_control_exist(S32 category_id, mmi_dm_control_ids_enum control_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *control_set = NULL;
    S16 *coorindate_set = NULL;
    S32 i, num_control = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    control_set = dm_search_control_set((U16) g_dm_data.S32CatId, &coorindate_set);
    if (!control_set)
    {
        return MMI_FALSE;
    }

    num_control = (S32) control_set[0];

    for (i = 1; i <= num_control; i++)
    {
        if (control_set[i] == (U8) control_id)
        {
            return MMI_TRUE;
        }
    }
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  dm_redraw_category_screen
 * DESCRIPTION
 *  This function is the actual Draw Manager, which is used by all categories to draw themselves.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#if 1//gdm 080528 fix bug8079
extern BOOL  gIsInSleepMode;
#endif
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
extern U8 change_window_flag;
extern void CSD_UI_get_new_window();
extern void CSD_UI_change_window(void);
#endif
//cat 151/9 used by some wait screen
BOOL dm_is_pop_category_screen(void)
{
#if defined(__MMI_NOKIA_STYLE_N800__)

     if(  ( POPUP_SCREENID == g_dm_data.S32ScrId ||
        MMI_CATEGORY72_ID == g_dm_data.S32CatId ||
        MMI_CATEGORY66_NORMAL_ID == g_dm_data.S32CatId ||
        MMI_CATEGORY66_STATUS_ICON_ID == g_dm_data.S32CatId ||
        MMI_CATEGORY66_NONE_ID == g_dm_data.S32CatId 
        || MMI_CATEGORY151_ID == g_dm_data.S32CatId
        || MMI_CATEGORY9_ID == g_dm_data.S32CatId
        )
    //    &&!mmi_frm_is_screen_width_height_swapped()
        )
     {
        return MMI_TRUE;
     }
     else
#endif
     {
        return MMI_FALSE;
     }


}
extern U8 arrived_idle_enablechange_window_flag;
extern U8 enablechange_window_flag;
void dm_redraw_category_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 u8CtrlCt = 0;
    S32 u8NoOfUICtrls = 0;
    S16 *UICtrlAccessPtr_p = NULL, *DeafultCoordinateSet_p = NULL;
    dm_cat_scr_info_struct dm_cat_scr_info;
    S32 idx = 0;
    U8 *control_set_ptr = NULL;
    S32 string_count = 0, image_count = 0, back_fill_count = 0, rectangle_count = 0, line_count =
        0, slide_control_count = 0;
    MMI_BOOL scr_bg_layer_exist = MMI_FALSE;
    S32 button_count = 0;
    S32 scroll_text_count = 0;
    S32 percentage_bar_count = 0;
    dm_redraw_state_enum redraw_type = NORMAL_EXIT;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(__MMI_SCREEN_ROTATE__) && !defined(__MMI_WALLPAPER_ON_BOTTOM__)
    if (mmi_frm_is_screen_width_height_swapped())
    {
        /* We always use another background layer if screen is rotated */
        scr_bg_layer_exist = MMI_TRUE;
    }
#endif /* defined(__MMI_SCREEN_ROTATE__) && !defined(__MMI_WALLPAPER_ON_BOTTOM__) */ 
#if defined(__MMI_LIST_BG_ON_BOTTOM_FOR_FAST_ENTRY_MENU__) && defined(__MMI_WALLPAPER_ON_BOTTOM__)
    wgui_set_wallpaper_on_bottom(MMI_TRUE);
    scr_bg_layer_exist = MMI_TRUE;
#endif 


#ifdef MMI_ON_HARDWARE_P
    while(hal_GoudaIsActive())
    {
        sxr_Sleep(32);
    }
#endif

#ifdef __MMI_SUBLCD__
    if (g_dm_data.S32flags & DM_SUB_LCD_CONTEXT)
    {
        if (g_dm_cntx.lock_redraw_sub == MMI_TRUE)
        {
            return;
        }
        g_dm_cntx.lock_redraw_sub = MMI_TRUE;

        switch (g_dm_cntx.redraw_screen_sub)
        {
                /* from new screen */
            case NORMAL_SHOW:
                g_dm_cntx.redraw_screen_sub = NORMAL_REDRAW;
                redraw_type = NORMAL_REDRAW;
                break;

                /* from show categoryscreen without exit screen */
            case REDRAW_2_SHOW:
                MMI_ASSERT(0);
                break;

                /* from redraw screen */
            case REDRAW_2_REDRAW:
                redraw_type = REDRAW_2_REDRAW;
                break;

            case NORMAL_REDRAW:
                g_dm_cntx.redraw_screen_sub = REDRAW_2_REDRAW;
                redraw_type = REDRAW_2_REDRAW;
                break;
        }

    }
    else
#endif /* __MMI_SUBLCD__ */ 
    {
        if (g_dm_cntx.lock_redraw_main == MMI_TRUE)
        {
            return;
        }
        g_dm_cntx.lock_redraw_main = MMI_TRUE;

        switch (g_dm_cntx.redraw_screen_main)
        {
                /* from new screen */
            case NORMAL_SHOW:
                g_dm_cntx.redraw_screen_main = NORMAL_REDRAW;
                redraw_type = NORMAL_REDRAW;
                break;

                /* from show categoryscreen without exit screen */
            case REDRAW_2_SHOW:
                /*
                 * dm_reset_context();
                 * g_dm_cntx.redraw_screen_main = NORMAL_REDRAW;
                 * redraw_type=NORMAL_REDRAW;
                 */
                MMI_ASSERT(0);
                break;

                /* from redraw screen */
            case REDRAW_2_REDRAW:
                redraw_type = REDRAW_2_REDRAW;
                break;

            case NORMAL_REDRAW:
                g_dm_cntx.redraw_screen_main = REDRAW_2_REDRAW;
                redraw_type = REDRAW_2_REDRAW;
                break;
			case NORMAL_EXIT:
				break;
        }
    }

    g_dm_cntx.is_context_reset = MMI_FALSE;

    memset((void*)&prev_control_coordinates, 0, sizeof(prev_control_coordinates));

    control_set_ptr = dm_search_control_set((U16) g_dm_data.S32CatId, &DeafultCoordinateSet_p);

    /* First element in Category-Controls map specifies the number of controls */
    u8NoOfUICtrls = control_set_ptr[0];

    UICtrlAccessPtr_p = dm_search_coordinate_set(g_dm_data.S32ScrId);
    if (UICtrlAccessPtr_p == NULL)
    {
        UICtrlAccessPtr_p = DeafultCoordinateSet_p;
    }

   /*************************************************************************
   If dm_search_coordinate_set returns NULL, then there must be Default Coordinate Set.
   *************************************************************************/
    MMI_ASSERT(UICtrlAccessPtr_p != NULL);

    gdi_layer_lock_frame_buffer();

    /* Get category screen size */
    /* Some category screens size are not the same as lcd size */
    UICtrlAccessPtr_p = dm_get_cat_scr_coordinates(UICtrlAccessPtr_p, &dm_cat_scr_info);
    /* Set clip according to category screen size */

    /* Assume active layer is base layer for mainlcd or sublcd */
        gdi_layer_set_active(GDI_LAYER_MAIN_BASE_LAYER_HANDLE);
    /* This might be an issue */

    gui_set_clip(dm_cat_scr_info.x1, dm_cat_scr_info.y1, dm_cat_scr_info.x2, dm_cat_scr_info.y2);

    if(!dm_is_pop_category_screen())
     {
        if (g_dm_data.S32flags & DM_CLEAR_SCREEN_BACKGROUND)
        {
        /*+ zhouqin 2011-02-16 modify for q6 320X240 */
        #if defined(__MMI_MAINLCD_220X176__) || defined(__MMI_MAINLCD_320X240__)
        /*- zhouqin 2011-02-16 modify for q6 320X240 */
		if(g_dm_data.S32CatId == MMI_CATEGORY129_ID)
		{
			gdi_draw_solid_rect(0, 0, UI_device_width - 1, UI_device_height - 1, GDI_COLOR_TRANSPARENT);
		}
		else
		{

			gdi_image_draw_id(0,0,IMG_MAIN_MENU_BG1);
		}
#else
            gdi_draw_solid_rect(0, 0, UI_device_width - 1, UI_device_height - 1, GDI_COLOR_TRANSPARENT);
#endif
        }
    }

#ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
    g_mmi_frm_cntx.dump_screen_info.allow_dump_dm_screen = MMI_TRUE;
#endif


    for (u8CtrlCt = 1; u8CtrlCt <= u8NoOfUICtrls; u8CtrlCt++)
    {
        switch (control_set_ptr[u8CtrlCt])
        {
            case DM_BASE_LAYER_START:
            {
                if (redraw_type != REDRAW_2_REDRAW)
                {
                    dm_setup_base_layer();
                }
                break;
            }
            case DM_NEW_LAYER_START:
            {
                dm_setup_new_layer(&UICtrlAccessPtr_p);
                break;
            }
	     case DM_ANIMATION_LAYER_DISPLAY:
			{
			#ifdef __MMI_ANIMATION_DISPALY__
			dm_setup_new_layer_for_animation_display(&UICtrlAccessPtr_p);
			#endif
		   break;
            }
            case DM_NEW_LAYER_END:  /* Just for Test, should be BASE_CONTROL_SET */
            {
                break;
            }
            case DM_SCR_BG:
            {
                 
            #if defined(__MMI_WALLPAPER_ON_BOTTOM__)
                dm_setup_and_draw_scr_bg(&UICtrlAccessPtr_p);
                scr_bg_layer_exist = MMI_TRUE;   
            #endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
                break;
                 
            }
#ifdef __SG_MMI_MEDIA_SPECIAL_MENU__
   	     case DM_MEDIA_MENU:
	     {
			dm_setup_and_draw_meida_menu(&UICtrlAccessPtr_p);
			break;
            }
#endif			
            case DM_CIRCULAR_MENU1:
            {
#if((ENABLE_CIRCULAR_3D_MENU) || (ENABLE_ROTATE_MENU))				
                dm_setup_and_draw_circular_menu(&UICtrlAccessPtr_p);
#endif
                break;
            }
            case DM_LIST1:
            {
                dm_setup_and_draw_fixed_list(&UICtrlAccessPtr_p);
                break;
            }
            case DM_DYNAMIC_LIST1:
            {
                dm_setup_and_draw_dynamic_list(&UICtrlAccessPtr_p);
                break;
            }
            case DM_ASYNCDYNAMIC_LIST1:
            {
                dm_setup_and_draw_asyncdynamic_list(&UICtrlAccessPtr_p);
                break;
            }
            case DM_MATRIX_MENU1:
            {
                dm_setup_and_draw_matrix_menu(&UICtrlAccessPtr_p);
                break;
            }
            case DM_MULTILINE_INPUTBOX1:
            {
                dm_setup_and_draw_multiline_inputbox(&UICtrlAccessPtr_p, &dm_cat_scr_info);
                break;
            }
            case DM_EMS_INPUTBOX1:
            {
                dm_setup_and_draw_ems_inputbox(&UICtrlAccessPtr_p, &dm_cat_scr_info);
                break;
            }
            case DM_DIALER_INPUT_BOX1:
            {
                dm_setup_and_draw_dialer_inputbox(&UICtrlAccessPtr_p);
                break;
            }
            case DM_SINGLELINE_INPUTBOX1:
            {
                dm_setup_and_draw_singleline_inputbox(&UICtrlAccessPtr_p);
                break;
            }
            case DM_LSK:
            {
                dm_setup_and_draw_left_softkey(&UICtrlAccessPtr_p);
                break;
            }
            case DM_RSK:
            {
                dm_setup_and_draw_right_softkey(&UICtrlAccessPtr_p);
                break;
            }
            case DM_BUTTON:
            {
                dm_setup_and_draw_button(&UICtrlAccessPtr_p, button_count++);
                break;
            }
            case DM_TITLE1:
            {
                dm_setup_and_draw_title(&UICtrlAccessPtr_p);
                break;
            }
		#ifdef __MMI_UI_TAB_PANE__
            case DM_HORIZONTAL_TAB_BAR:
            {
                dm_setup_and_draw_horizontal_tab_bar(&UICtrlAccessPtr_p);
                break;
            }
		#endif
            case DM_CATEGORY_CONTROLLED_AREA:
            {
                dm_setup_and_draw_category_controlled_area(&UICtrlAccessPtr_p);
                break;
            }
            case DM_CATEGORY_CONTROLLED_AREA2:
            {
                dm_setup_and_draw_category_controlled_area2(&UICtrlAccessPtr_p);
                break;
            }
            case DM_STATUS_BAR1:
            {
                dm_setup_and_draw_status_icons(&UICtrlAccessPtr_p);
                break;
            }
            case DM_BUTTON_BAR1:
            {
                dm_setup_and_draw_button_bar(&UICtrlAccessPtr_p);
                break;
            }
            case DM_INLINE_FIXED_LIST1:
            {
                dm_setup_and_draw_inline_item(&UICtrlAccessPtr_p);
                break;
            }
            case DM_BASE_CONTROL_SET1:
            {
                dm_setup_and_draw_base_control_set(&UICtrlAccessPtr_p);
                break;
            }
            case DM_BASE_CONTROL_SET2:
            {
                dm_setup_and_draw_base_control_set2(&UICtrlAccessPtr_p);
                break;
            }
            case DM_STRING:
            {
                dm_setup_and_draw_string(&UICtrlAccessPtr_p, string_count++);
                break;
            }
            case DM_IMAGE:
            {
                dm_setup_and_draw_image(&UICtrlAccessPtr_p, image_count++);
                break;
            }
            case DM_CALENDAR:
            {
                dm_setup_and_draw_calendar(&UICtrlAccessPtr_p);
                break;
            }
            case DM_ALIGNED_AREA_START:
            {
                dm_aligned_area_data.is_in_aligned_area = MMI_TRUE;
                dm_setup_and_draw_aligned_area(&UICtrlAccessPtr_p, control_set_ptr);
                break;
            }
            case DM_ALIGNED_AREA_END:
            {
                memset(&dm_aligned_area_data, 0, sizeof(dm_aligned_area_data));
                dm_aligned_area_data.is_in_aligned_area = MMI_FALSE;
                break;
            }
            case DM_DATE_TIME_DISPLAY:
            {
            #ifdef __MMI_SUBLCD__
                if (g_dm_data.S32flags & DM_SUB_LCD_CONTEXT)
                    if (redraw_type == REDRAW_2_REDRAW)
                    {
                        update_sublcd_dt_display();
                    }
                    else
                    {
                        show_sub_LCD_dt_display();
                    }
                else
            #endif /* __MMI_SUBLCD__ */ 
                if (redraw_type == REDRAW_2_REDRAW)
                {
                    update_mainlcd_dt_display();
                }
                else
                {
                    show_main_LCD_dt_display();
                }
                break;
            }
            case DM_BACK_FILL_AREA:
            {
                dm_setup_and_draw_back_fill_area(&UICtrlAccessPtr_p, back_fill_count++);
                break;
            }
            case DM_RECTANGLE:
            {
                dm_setup_and_draw_rectangle(&UICtrlAccessPtr_p, rectangle_count++);
                break;
            }
            case DM_LINE:
            {
                dm_setup_and_draw_line(&UICtrlAccessPtr_p, line_count++);
                break;
            }
            case DM_SLIDE_CONTROL:
            {
                dm_setup_and_draw_slide_control(&UICtrlAccessPtr_p, slide_control_count++);
                break;
            }
            case DM_POPUP_BACKGROUND:
            {
                dm_setup_and_draw_popup_background(&UICtrlAccessPtr_p);
                break;
            }
            case DM_TYPICAL_STOPWATCH:
            {
                dm_setup_and_draw_typical_stopwatch(&UICtrlAccessPtr_p);
                break;
            }
            case DM_NWAY_STOPWATCH:
            {
                dm_setup_and_draw_nway_stopwatch(&UICtrlAccessPtr_p);
                break;
            }
	      case DM_VOLUME_LEVEL_DISPLAY:
		 {
		dm_setup_and_draw_volume_level(&UICtrlAccessPtr_p);
		break;
	     	}
            case DM_WALL_PAPER:
            {
				//qiff delete for greenstone bug:9446
				#if 0//gdm 080528 fix bug8079
				mmi_trace(g_sw_WpSsPwon, "gdm:call dm_setup_and_draw_wallpaper()gIsInSleepMode=%d", gIsInSleepMode);
				if(1 == gIsInSleepMode)
					break;
				#endif
                dm_setup_and_draw_wallpaper(&UICtrlAccessPtr_p);
                break;
            }
            case DM_SCROLL_TEXT:
            {
                dm_setup_and_draw_scroll_text(&UICtrlAccessPtr_p, scroll_text_count++);
                break;
            }
            case DM_PERCENTAGE_BAR:
            {
                dm_setup_and_draw_percentage_bar(&UICtrlAccessPtr_p, percentage_bar_count++);
            }

        }
    }
#ifndef __GDI_MEMORY_PROFILE_2__
    new_layer_x = 0;
    new_layer_y = 0;
#endif /* __GDI_MEMORY_PROFILE_2__ */ 

    /* if virtual keypad show flag is set, show the virtual keypad */
    if (g_dm_data.S32flags & DM_SHOW_VKPAD)
    {
        if (dm_show_vkpad_callback != NULL)
        {
            dm_show_vkpad_callback();
        }
        /* wgui_show_virtual_keyboard(); */
    }
    gdi_layer_restore_base_active();

    if (scr_bg_layer_exist == MMI_FALSE)
    {
        for (idx = 0; idx < GDI_LAYER_TOTAL_LAYER_COUNT; idx++)
        {
            if (dm_layers[idx] == g_dm_scr_bg_cntx.layer_handle)
            {
                dm_layers[idx] = 0;
                break;
            }
        }
        if (idx < GDI_LAYER_TOTAL_LAYER_COUNT)
        {
            dm_layers[idx] = g_dm_scr_bg_cntx.layer_handle;
        }
    }

    gdi_layer_unlock_frame_buffer();
#ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
if((arrived_idle_enablechange_window_flag)&&(enablechange_window_flag))
{
gdi_layer_set_blt_layer(dm_layers[0], dm_layers[1], dm_layers[2], dm_layers[3]);
CSD_UI_get_new_window();
if((g_dm_data.S32CatId != MMI_CATEGORY221_ID)&&(MMI_CATEGORY64_ID != g_dm_data.S32CatId))
{
    CSD_UI_change_window();
}
}
if(change_window_flag == 0)
#endif
{
    if (!(g_dm_data.S32flags & DM_NO_BLT))
    {
        gdi_layer_blt(
            dm_layers[0],
            dm_layers[1],
            dm_layers[2],
            dm_layers[3],
            dm_cat_scr_info.x1,
            dm_cat_scr_info.y1,
            dm_cat_scr_info.x2 - dm_cat_scr_info.x1 - 1,
            dm_cat_scr_info.y2 - dm_cat_scr_info.y1 - 1);
    }
    else
    {
        gdi_layer_set_blt_layer(dm_layers[0], dm_layers[1], dm_layers[2], dm_layers[3]);
    }
}
    /* 061506 double buffer Start */
    for (idx=0; idx<MAX_STATUS_ICON_BARS; idx++)
    {
        if(get_status_icon_bar_type(idx)&STATUS_ICON_BAR_ENABLE_DOUBLE_BUFFER)
        {
            gdi_layer_push_and_set_active(get_status_icon_bar_layer(idx));
            gdi_layer_toggle_double();
            gdi_layer_copy_double();//070306 copy double
            gdi_layer_pop_and_restore_active();
        }
    }
    /* 061506 double buffer Start */

#ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
    g_mmi_frm_cntx.dump_screen_info.allow_dump_dm_screen = MMI_FALSE;
#endif

    g_dm_cntx.lock_redraw_main = MMI_FALSE;
}   /* end of dm_redraw_category_screen */


/* Get Coordinate Set for different components */


/*****************************************************************************
 * FUNCTION
 *  dm_get_coordinates
 * DESCRIPTION
 *  To get coordinates for any control.
 * PARAMETERS
 *  UICtrlAccessPtr_p       [?]     
 *  dm_coordinate_info      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
S16 *dm_get_coordinates(S16 *UICtrlAccessPtr_p, dm_coordinates *dm_coordinate_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (*UICtrlAccessPtr_p == DM_DUMMY_COORDINATE)
    {
        dm_coordinate_info->S16X = 0;
        dm_coordinate_info->S16Y = 0;
        dm_coordinate_info->S16Width = 0;
        dm_coordinate_info->S16Height = 0;
        dm_coordinate_info->Flags = DM_NO_FLAGS;
        UICtrlAccessPtr_p++;
    }
    else if (*UICtrlAccessPtr_p == DM_NULL_COORDINATE)
    {
        dm_coordinate_info->S16X = -1;
        dm_coordinate_info->S16Y = -1;
        dm_coordinate_info->S16Width = -1;
        dm_coordinate_info->S16Height = -1;
        dm_coordinate_info->Flags = DM_NO_FLAGS;
        UICtrlAccessPtr_p++;
    }
    else if (*UICtrlAccessPtr_p == DM_FULL_SCREEN_COORDINATE_FLAG || *UICtrlAccessPtr_p == DM_FULL_SCREEN_COORDINATE)
    {
    #ifdef __MMI_SCREEN_ROTATE__
        if (mmi_frm_is_screen_width_height_swapped())
        {
            dm_coordinate_info->S16X = 0;
            dm_coordinate_info->S16Y = 0;
            dm_coordinate_info->S16Width = MMI_ROTATED_LCD_WIDTH;
            dm_coordinate_info->S16Height = MMI_ROTATED_LCD_HEIGHT;
        }
        else
    #endif /* __MMI_SCREEN_ROTATE__ */ 
        {
            dm_coordinate_info->S16X = 0;
            dm_coordinate_info->S16Y = 0;
            dm_coordinate_info->S16Width = MAIN_LCD_DEVICE_WIDTH;
            dm_coordinate_info->S16Height = MAIN_LCD_DEVICE_HEIGHT;
        }

        if (*UICtrlAccessPtr_p == DM_FULL_SCREEN_COORDINATE_FLAG)
        {
            dm_coordinate_info->Flags = DM_NO_FLAGS;
        }
        else
        {
            UICtrlAccessPtr_p++;
            dm_coordinate_info->Flags = *UICtrlAccessPtr_p;
        }

        UICtrlAccessPtr_p++;
    }
    else if (*UICtrlAccessPtr_p == DM_CONTENT_COORDINATE_FLAG || *UICtrlAccessPtr_p == DM_CONTENT_COORDINATE)
    {
    #ifdef __MMI_SCREEN_ROTATE__
        if (mmi_frm_is_screen_width_height_swapped())
        {
            dm_coordinate_info->S16X = MMI_ROTATED_CONTENT_X;
            dm_coordinate_info->S16Y = MMI_ROTATED_CONTENT_Y;
            dm_coordinate_info->S16Width = MMI_ROTATED_CONTENT_WIDTH;
            dm_coordinate_info->S16Height = MMI_ROTATED_CONTENT_HEIGHT;
        }
        else
    #endif /* __MMI_SCREEN_ROTATE__ */ 
        {
            dm_coordinate_info->S16X = MMI_CONTENT_X;
            dm_coordinate_info->S16Y = MMI_CONTENT_Y;
            dm_coordinate_info->S16Width = MMI_CONTENT_WIDTH;
            dm_coordinate_info->S16Height = MMI_CONTENT_HEIGHT;
        }

        if (*UICtrlAccessPtr_p == DM_CONTENT_COORDINATE_FLAG)
        {
            dm_coordinate_info->Flags = DM_NO_FLAGS;
        }
        else
        {
            UICtrlAccessPtr_p++;
            dm_coordinate_info->Flags = *UICtrlAccessPtr_p;
        }
        UICtrlAccessPtr_p++;
    }
    else if (*UICtrlAccessPtr_p == DM_POPUP_SCREEN_COORDINATE_FLAG || *UICtrlAccessPtr_p == DM_POPUP_SCREEN_COORDINATE)
    {
    #ifdef __MMI_SCREEN_ROTATE__
        if (mmi_frm_is_screen_width_height_swapped())
        {
            dm_coordinate_info->S16X = MMI_ROTATED_POP_UP_DIALOG_X;
            dm_coordinate_info->S16Y = MMI_ROTATED_POP_UP_DIALOG_Y;
            dm_coordinate_info->S16Width = MMI_ROTATED_POP_UP_DIALOG_WIDTH;
            dm_coordinate_info->S16Height = MMI_ROTATED_POP_UP_DIALOG_FULL_HEIGHT;
        }
        else
    #endif /* __MMI_SCREEN_ROTATE__ */ 
        {
            dm_coordinate_info->S16X = MMI_POP_UP_DIALOG_X;
            dm_coordinate_info->S16Y = MMI_POP_UP_DIALOG_Y;
            dm_coordinate_info->S16Width = MMI_POP_UP_DIALOG_WIDTH;
#if (defined(__MMI_MAINLCD_128X160__)||defined (__MMI_MAINLCD_128X128__)) && defined(MEM_ULC_3216)
            dm_coordinate_info->S16Height = MMI_POP_UP_DIALOG_HEIGHT;
#else
			dm_coordinate_info->S16Height = MMI_POP_UP_DIALOG_FULL_HEIGHT;
#endif
        }

        if (*UICtrlAccessPtr_p == DM_POPUP_SCREEN_COORDINATE_FLAG)
        {
            dm_coordinate_info->Flags = DM_NO_FLAGS;
        }
        else
        {
            UICtrlAccessPtr_p++;
            dm_coordinate_info->Flags = *UICtrlAccessPtr_p;
        }
        UICtrlAccessPtr_p++;
    }
    else if (*UICtrlAccessPtr_p == DM_DEFAULT_TITLE_BAR_FLAG || *UICtrlAccessPtr_p == DM_DEFAULT_TITLE_BAR)
    {
    #ifdef __MMI_SCREEN_ROTATE__
        if (mmi_frm_is_screen_width_height_swapped())
        {
            dm_coordinate_info->S16X = MMI_ROTATED_TITLE_X;
            dm_coordinate_info->S16Y = MMI_ROTATED_TITLE_Y;
            dm_coordinate_info->S16Width = MMI_ROTATED_TITLE_WIDTH;
            dm_coordinate_info->S16Height = MMI_ROTATED_TITLE_HEIGHT;
        }
        else
    #endif /* __MMI_SCREEN_ROTATE__ */ 
        {
            dm_coordinate_info->S16X = MMI_TITLE_X;
            dm_coordinate_info->S16Y = MMI_TITLE_Y;
            dm_coordinate_info->S16Width = MMI_TITLE_WIDTH;
            dm_coordinate_info->S16Height = MMI_TITLE_HEIGHT;
        }

        if (*UICtrlAccessPtr_p == DM_DEFAULT_TITLE_BAR_FLAG)
        {
            dm_coordinate_info->Flags = DM_NO_FLAGS;
        }
        else
        {
            UICtrlAccessPtr_p++;
            dm_coordinate_info->Flags = *UICtrlAccessPtr_p;
        }

        UICtrlAccessPtr_p++;
    }
    else if (*UICtrlAccessPtr_p == DM_DEFAULT_BUTTON_BAR_FLAG || *UICtrlAccessPtr_p == DM_DEFAULT_BUTTON_BAR)
    {
    #ifdef __MMI_SCREEN_ROTATE__
        if (mmi_frm_is_screen_width_height_swapped())
        {
            dm_coordinate_info->S16X = MMI_ROTATED_BUTTON_BAR_X;
            dm_coordinate_info->S16Y = MMI_ROTATED_BUTTON_BAR_Y;
            dm_coordinate_info->S16Width = MMI_ROTATED_BUTTON_BAR_WIDTH;
            dm_coordinate_info->S16Height = MMI_ROTATED_BUTTON_BAR_HEIGHT;
        }
        else
    #endif /* __MMI_SCREEN_ROTATE__ */ 
        {
            dm_coordinate_info->S16X = 0;
 
        #if defined (__MMI_UI_DALMATIAN_SOFTKEYS__)||defined(__MMI_MAINLCD_220X176__)
            dm_coordinate_info->S16Y = MAIN_LCD_DEVICE_HEIGHT - MMI_BUTTON_BAR_HEIGHT;
        #else 
            dm_coordinate_info->S16Y = MAIN_LCD_DEVICE_HEIGHT - MMI_BUTTON_BAR_HEIGHT + 1;
        #endif 
  
            dm_coordinate_info->S16Width = MAIN_LCD_DEVICE_WIDTH;
            dm_coordinate_info->S16Height = MMI_BUTTON_BAR_HEIGHT;
        }

        if (*UICtrlAccessPtr_p == DM_DEFAULT_BUTTON_BAR_FLAG)
        {
            dm_coordinate_info->Flags = DM_NO_FLAGS;
        }
        else
        {
            UICtrlAccessPtr_p++;
            dm_coordinate_info->Flags = *UICtrlAccessPtr_p;
        }
        UICtrlAccessPtr_p++;
    }
    else
    {
        dm_coordinate_info->S16X = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_coordinate_info->S16Y = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_coordinate_info->S16Width = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_coordinate_info->S16Height = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_coordinate_info->Flags = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;
    }

    if (dm_coordinate_info->Flags & DM_PREVIOUS_CONTROL_END_X)
    {
        dm_coordinate_info->S16X = prev_control_coordinates.S16X2 + dm_coordinate_info->S16X;
    }
    else if (dm_coordinate_info->Flags & DM_PREVIOUS_CONTROL_END_X_OFFSET_WIDTH)
    {
        dm_coordinate_info->S16X = prev_control_coordinates.S16X2 - dm_coordinate_info->S16Width;
    }
#ifndef __GDI_MEMORY_PROFILE_2__
    else
    {
        if (new_layer_x != 0)
        {
            dm_coordinate_info->S16X += new_layer_x;
        }
    }
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    if (dm_coordinate_info->Flags & DM_PREVIOUS_CONTROL_END_Y)
    {
        dm_coordinate_info->S16Y = prev_control_coordinates.S16Y2 + dm_coordinate_info->S16Y;
    }
    else if (dm_coordinate_info->Flags & DM_PREVIOUS_CONTROL_END_Y_OFFSET_HEIGHT)
    {
        dm_coordinate_info->S16Y = prev_control_coordinates.S16Y2 - dm_coordinate_info->S16Height;
    }
#ifndef __GDI_MEMORY_PROFILE_2__
    else
    {
        if (new_layer_y != 0)
        {
            dm_coordinate_info->S16Y += new_layer_y;
        }
    }
#endif /* __GDI_MEMORY_PROFILE_2__ */ 
    if (dm_coordinate_info->Flags & DM_WIDTH_OFFSET_PREVIOUS_CONTROL_END_X)
    {
        dm_coordinate_info->S16Width -= prev_control_coordinates.S16X2;
    }
    if (dm_coordinate_info->Flags & DM_HEIGHT_OFFSET_PREVIOUS_CONTROL_END_Y)
    {
        dm_coordinate_info->S16Height -= prev_control_coordinates.S16Y2;
    }
    if (dm_coordinate_info->Flags & DM_HEIGHT_PREVIOUS_CONTROL_HEIGHT)
    {
        dm_coordinate_info->S16Height = prev_control_coordinates.coordinate.S16Height;
    }
    if (dm_coordinate_info->Flags & DM_WIDTH_PREVIOUS_CONTROL_WIDTH)
    {
        dm_coordinate_info->S16Width = prev_control_coordinates.coordinate.S16Width;
    }

    setup_previous_control(dm_coordinate_info);

    return UICtrlAccessPtr_p;
}


/*****************************************************************************
 * FUNCTION
 *  dm_get_cat_scr_coordinates
 * DESCRIPTION
 *  This function is used obtain the category screen coordinates
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  dm_cat_scr_info         [IN/OUT]        Category screen coordinate info.
 * RETURNS
 *  void
 *****************************************************************************/
S16 *dm_get_cat_scr_coordinates(S16 *UICtrlAccessPtr_p, dm_cat_scr_info_struct *dm_cat_scr_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Check if full screen display */
    if ((*UICtrlAccessPtr_p == DM_FULL_SCREEN_COORDINATE_FLAG) || (*UICtrlAccessPtr_p == DM_FULL_SCREEN_COORDINATE))
    {
        dm_cat_scr_info->x1 = 0;
        dm_cat_scr_info->y1 = 0;
        dm_cat_scr_info->x2 = UI_device_width;
        dm_cat_scr_info->y2 = UI_device_height;

        if (*UICtrlAccessPtr_p == DM_FULL_SCREEN_COORDINATE_FLAG)
        {
            dm_cat_scr_info->flags = DM_NO_FLAGS;
            UICtrlAccessPtr_p++;
            return UICtrlAccessPtr_p;
        }
        UICtrlAccessPtr_p++;
    }
    else
    {
        dm_cat_scr_info->x1 = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_cat_scr_info->y1 = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_cat_scr_info->x2 = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_cat_scr_info->y2 = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;
    }

    dm_cat_scr_info->flags = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    return UICtrlAccessPtr_p;
}   /* end of dm_get_cat_scr_coordinates */


/*****************************************************************************
 * FUNCTION
 *  dm_get_status_icon_bar_coordinates
 * DESCRIPTION
 *  This function is used obtain the status icon bar coordinates.
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  dm_status_info          [IN/OUT]        Status bar coordinate info.
 * RETURNS
 *  void
 *****************************************************************************/
S16 *dm_get_status_icon_bar_coordinates(S16 *UICtrlAccessPtr_p, dm_status_info_struct *dm_status_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (*UICtrlAccessPtr_p == DM_DEFAULT_STATUS_BAR_FLAG || *UICtrlAccessPtr_p == DM_DEFAULT_STATUS_BAR)
    {
        /* status bar is not suported in rotated screen */
        dm_status_info->x = 0;
        dm_status_info->y = 0;
        dm_status_info->x1 = 0;
        dm_status_info->y1 = 0;
        dm_status_info->x2 = MAIN_LCD_DEVICE_WIDTH;
        dm_status_info->y2 = MMI_STATUS_BAR_HEIGHT;
        if (*UICtrlAccessPtr_p == DM_DEFAULT_STATUS_BAR_FLAG)
        {
            dm_status_info->flags = DM_NO_FLAGS;
        }
        else
        {
            UICtrlAccessPtr_p++;
            dm_status_info->flags = *UICtrlAccessPtr_p;
        }

        UICtrlAccessPtr_p++;
    }
    else
    {
        dm_status_info->x = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_status_info->y = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_status_info->x1 = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_status_info->y1 = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_status_info->x2 = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_status_info->y2 = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;

        dm_status_info->flags = *UICtrlAccessPtr_p;
        UICtrlAccessPtr_p++;
    }

    return UICtrlAccessPtr_p;
}   /* end of dm_get_status_icon_bar_coordinates */


/*****************************************************************************
 * FUNCTION
 *  dm_get_button_bar_coordinates
 * DESCRIPTION
 *  This function is used obtain the button bar coordinates.
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  dm_buttonbar_info       [IN/OUT]        Button bar coordinate info.
 * RETURNS
 *  void
 *****************************************************************************/
S16 *dm_get_button_bar_coordinates(S16 *UICtrlAccessPtr_p, dm_buttonbar_info_struct *dm_buttonbar_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, &dm_buttonbar_info->coordinate);

    dm_buttonbar_info->button_width = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;
    return UICtrlAccessPtr_p;
}   /* end of dm_get_button_bar_coordinates */


/*****************************************************************************
 * FUNCTION
 *  dm_get_calendar_coordinates
 * DESCRIPTION
 *  This function is used obtain the calendar control coordinates.
 * PARAMETERS
 *  UICtrlAccessPtr_p       [IN/OUT]        It specifies the start position to take out the coordinates.
 *  dm_calendar_info        [IN/OUT]        Calendar control coordinate info.
 * RETURNS
 *  void
 *****************************************************************************/
S16 *dm_get_calendar_coordinates(S16 *UICtrlAccessPtr_p, dm_calendar_info_struct *dm_calendar_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, &dm_calendar_info->coordinate);

    dm_calendar_info->calendar_background_y = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->calendar_background_width = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->calendar_columns = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->calendar_rows = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title1_x = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title1_y = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title1_width = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title1_height = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title1_font = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    dm_calendar_info->title1_month_x = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title1_month_y = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title1_month_width = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title1_month_height = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title1_month_font = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title1_month_show = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

    dm_calendar_info->title2_x = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title2_y = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title2_width = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title2_height = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title2_show = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title3_x = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title3_y = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title3_width = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title3_height = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->title3_show = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

#ifdef __MMI_UI_CALENDAR_WITH_INFO_BOX__
    dm_calendar_info->infobox_x = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->infobox_y = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->infobox_width = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->infobox_height = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->infobox_max = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_calendar_info->infobox_show = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;
#endif /* __MMI_UI_CALENDAR_WITH_INFO_BOX__ */ 

    return UICtrlAccessPtr_p;
}


/*****************************************************************************
 * FUNCTION
 *  dm_get_typical_stopwatch_coordinates
 * DESCRIPTION
 *  This function is used obtain the typical stopwatch control coordinates.
 * PARAMETERS
 *  UICtrlAccessPtr_p               [IN/OUT]        It specifies the start position to take out the coordinates.
 *  dm_typical_stopwatch_info       [?]             
 *  dm_calendar_info(?)             [IN/OUT]        Calendar control coordinate info.
 * RETURNS
 *  void
 *****************************************************************************/
S16 *dm_get_typical_stopwatch_coordinates(
        S16 *UICtrlAccessPtr_p,
        dm_typical_stopwatch_info_struct *dm_typical_stopwatch_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, &dm_typical_stopwatch_info->coordinate);

    dm_typical_stopwatch_info->typ_timer_x = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_typical_stopwatch_info->typ_timer_y = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_typical_stopwatch_info->typ_milsec_x = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_typical_stopwatch_info->typ_milsec_y = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_typical_stopwatch_info->typ_history_height = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_typical_stopwatch_info->typ_timer_x += dm_typical_stopwatch_info->coordinate.S16X;
    dm_typical_stopwatch_info->typ_timer_y += dm_typical_stopwatch_info->coordinate.S16Y;

    dm_typical_stopwatch_info->typ_milsec_x += dm_typical_stopwatch_info->coordinate.S16X;
    dm_typical_stopwatch_info->typ_milsec_y += dm_typical_stopwatch_info->coordinate.S16Y;

    return UICtrlAccessPtr_p;
}


/*****************************************************************************
 * FUNCTION
 *  dm_get_nway_stopwatch_coordinates
 * DESCRIPTION
 *  This function is used obtain the N Way stopwatch control coordinates.
 * PARAMETERS
 *  UICtrlAccessPtr_p           [IN/OUT]        It specifies the start position to take out the coordinates.
 *  dm_nway_stopwatch_info      [?]             
 *  dm_calendar_info(?)         [IN/OUT]        Calendar control coordinate info.
 * RETURNS
 *  void
 *****************************************************************************/
S16 *dm_get_nway_stopwatch_coordinates(S16 *UICtrlAccessPtr_p, dm_nway_stopwatch_info_struct *dm_nway_stopwatch_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, &dm_nway_stopwatch_info->coordinate);

    dm_nway_stopwatch_info->nway_main_timer_x1 = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_nway_stopwatch_info->nway_main_timer_y1 = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_nway_stopwatch_info->nway_main_timer_x2 = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_nway_stopwatch_info->nway_main_timer_y2 = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_nway_stopwatch_info->nway_dialog_x1 = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_nway_stopwatch_info->nway_dialog_y1 = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_nway_stopwatch_info->nway_dialog_x2 = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_nway_stopwatch_info->nway_dialog_y2 = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_nway_stopwatch_info->separator_height = *UICtrlAccessPtr_p;
    UICtrlAccessPtr_p++;

    dm_nway_stopwatch_info->nway_main_timer_x1 += dm_nway_stopwatch_info->coordinate.S16X;
    dm_nway_stopwatch_info->nway_main_timer_y1 += dm_nway_stopwatch_info->coordinate.S16Y;
    dm_nway_stopwatch_info->nway_main_timer_x2 += dm_nway_stopwatch_info->coordinate.S16X;
    dm_nway_stopwatch_info->nway_main_timer_y2 += dm_nway_stopwatch_info->coordinate.S16Y;
    dm_nway_stopwatch_info->nway_dialog_x1 += dm_nway_stopwatch_info->coordinate.S16X;
    dm_nway_stopwatch_info->nway_dialog_y1 += dm_nway_stopwatch_info->coordinate.S16Y;
    dm_nway_stopwatch_info->nway_dialog_x2 += dm_nway_stopwatch_info->coordinate.S16X;
    dm_nway_stopwatch_info->nway_dialog_y2 += dm_nway_stopwatch_info->coordinate.S16Y;

    return UICtrlAccessPtr_p;
}


/*****************************************************************************
 * FUNCTION
 *  dm_get_category_history
 * DESCRIPTION
 *  This function is used obtain the category history.
 * PARAMETERS
 *  history_buffer      [IN/OUT]        Buffer of history data.
 * RETURNS
 *  U8 history
 *****************************************************************************/
U8 *dm_get_category_history(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 u8NoOfUICtrls = 0;
    U8 u8CtrlCt = 0;
    U8 *control_set_ptr = NULL;
    S16 *coordinate_set_p;
    dm_data_struct *p_dm_data = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_dm_data.S32CatId)
    {
        p_dm_data = &g_dm_data;
    }
    else if (g_dm_sublcd_data.S32CatId)
    {
        p_dm_data = &g_dm_sublcd_data;
    }
    else
    {
        MMI_ASSERT(0);
    }
    control_set_ptr = dm_search_control_set((U16) p_dm_data->S32CatId, &coordinate_set_p);
    /* First element in Category-Controls map specifies the number of controls */
    u8NoOfUICtrls = control_set_ptr[0];
    for (u8CtrlCt = 1; u8CtrlCt <= u8NoOfUICtrls; u8CtrlCt++)
    {
        switch (control_set_ptr[u8CtrlCt])
        {
            case DM_CIRCULAR_MENU1:
            {
#if((ENABLE_CIRCULAR_3D_MENU) || (ENABLE_ROTATE_MENU))				
                get_circular_menu_category_history((U16) p_dm_data->S32CatId, history_buffer);
#endif
                break;
            }
            case DM_LIST1:
            {
                get_list_menu_category_history((U16) p_dm_data->S32CatId, history_buffer);
                break;
            }
            case DM_DYNAMIC_LIST1:
            {
                get_list_menu_category_history((U16) p_dm_data->S32CatId, history_buffer);
                break;
            }
            case DM_ASYNCDYNAMIC_LIST1:
            {
                get_list_menu_category_history((U16) p_dm_data->S32CatId, history_buffer);
                break;
            }
            case DM_MATRIX_MENU1:
            {
                get_matrix_menu_category_history((U16) p_dm_data->S32CatId, history_buffer);
                break;
            }
            case DM_MULTILINE_INPUTBOX1:
            {
                get_multiline_inputbox_category_history(
                    (U16) p_dm_data->S32CatId,
                    history_buffer,
                    MMI_current_input_type);
                break;
            }
            case DM_EMS_INPUTBOX1:
            {
                get_EMS_inputbox_category_history((U16) p_dm_data->S32CatId, history_buffer, MMI_current_input_type);
                break;
            }
            case DM_DIALER_INPUT_BOX1:
            {
                get_dialer_inputbox_category_history((U16) p_dm_data->S32CatId, history_buffer);
                break;
            }
            case DM_SINGLELINE_INPUTBOX1:
            {
                get_singleline_inputbox_category_history(
                    (U16) p_dm_data->S32CatId,
                    history_buffer,
                    MMI_current_input_type);
                break;
            }
            case DM_LSK:
            {
                /* No history currently for this control */
                break;
            }
            case DM_RSK:
            {
                /* No history currently for this control */
                break;
            }
            case DM_TITLE1:
            {
                /* No history currently for this control */
                break;
            }
            case DM_STATUS_BAR1:
            {
                /* No history currently for this control */
                break;
            }
            case DM_INLINE_FIXED_LIST1:
            {
                wgui_get_inline_history((U16) p_dm_data->S32CatId, history_buffer);
                break;
            }
            case DM_SLIDE_CONTROL:
            {
                get_slide_control_category_history((U16) p_dm_data->S32CatId, history_buffer);
                break;
            }
            case DM_TYPICAL_STOPWATCH:
            {
                get_list_menu_category_history((U16) p_dm_data->S32CatId, history_buffer);
                break;
            }
        #ifdef __MMI_STOPWATCH__
            case DM_NWAY_STOPWATCH:
            {
                get_stop_watch_history((U16) p_dm_data->S32CatId, history_buffer);
                break;
            }
        #endif /* __MMI_STOPWATCH__ */ 
            default:
            {
                break;
            }
        }
    }
    return history_buffer;
}   /* end of dm_get_category_history */


/*****************************************************************************
 * FUNCTION
 *  dm_get_category_history_size
 * DESCRIPTION
 *  This function is used obtain the category history.
 * PARAMETERS
 *  void
 * RETURNS
 *  S32 history_size
 *****************************************************************************/
S32 dm_get_category_history_size(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 u8CtrlCt = -1;
    S32 u8NoOfUICtrls = -1;
    U8 *control_set_ptr = NULL;
    S16 *coordinate_set_ptr = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_dm_data.S32CatId)
    {
        control_set_ptr = dm_search_control_set((U16) g_dm_data.S32CatId, &coordinate_set_ptr);
    }
    else if (g_dm_sublcd_data.S32CatId)
    {
        control_set_ptr = dm_search_control_set((U16) g_dm_sublcd_data.S32CatId, &coordinate_set_ptr);
    }
    else
    {
        MMI_ASSERT(0);
    }
    /* First element in Category-Controls map specifies the number of controls */
    u8NoOfUICtrls = control_set_ptr[0];
    for (u8CtrlCt = 1; u8CtrlCt <= u8NoOfUICtrls; u8CtrlCt++)
    {
        switch (control_set_ptr[u8CtrlCt])
        {
            case DM_CIRCULAR_MENU1:
            {
                return (sizeof(circular_menu_category_history));
            }
            case DM_LIST1:
            {
                return (sizeof(list_menu_category_history));
            }
            case DM_DYNAMIC_LIST1:
            {
                return (sizeof(list_menu_category_history));
            }
            case DM_ASYNCDYNAMIC_LIST1:
            {
                return (sizeof(list_menu_category_history));
            }
            case DM_MATRIX_MENU1:
            {
                return (sizeof(matrix_menu_category_history));
            }
            case DM_MULTILINE_INPUTBOX1:
            {
                return (sizeof(multiline_inputbox_category_history));
            }
            case DM_DIALER_INPUT_BOX1:
            {
                return (sizeof(dialer_inputbox_category_history));
            }
            case DM_SINGLELINE_INPUTBOX1:
            {
                return (sizeof(singleline_inputbox_category_history));
            }
            case DM_LSK:
            {
                /* No history currently for this control */
                return 0;
            }
            case DM_RSK:
            {
                /* No history currently for this control */
                return 0;
            }
            case DM_TITLE1:
            {
                /* No history currently for this control */
                return 0;
            }
            case DM_STATUS_BAR1:
            {
                /* No history currently for this control */
                return 0;
            }
            case DM_INLINE_FIXED_LIST1:
            {
                return (wgui_get_inline_history_size());
            }
            case DM_SLIDE_CONTROL:
            {
                return (sizeof(slide_control_category_history));
            }
            case DM_TYPICAL_STOPWATCH:
            {
                return (sizeof(list_menu_category_history));
                break;
            }
        #ifdef __MMI_STOPWATCH__
            case DM_NWAY_STOPWATCH:
            {
                return (sizeof(UI_stop_watch_history));
                break;
            }
        #endif /* __MMI_STOPWATCH__ */ 
            default:
            {
                PRINT_INFORMATION(("WARNING: Can't find any control to calculate history_t size"));
                MMI_ASSERT(0);
                return 0;
            }
        }
    }
    return 0;
}   /* end of dm_get_category_history_size */


/*****************************************************************************
 * FUNCTION
 *  dm_reset_title_coordinates
 * DESCRIPTION
 *  Reset function for the title coordinates so that other category screens get the original values only.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_title_coordinates(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_SCREEN_ROTATE__
    if (mmi_frm_is_screen_width_height_swapped())
    {
        MMI_title_height = MMI_ROTATED_TITLE_HEIGHT;
        MMI_title_width = MMI_ROTATED_TITLE_WIDTH;
        MMI_title_x = MMI_ROTATED_TITLE_X;
        MMI_title_y = MMI_ROTATED_TITLE_Y;
    }
    else
#endif /* __MMI_SCREEN_ROTATE__ */ 
    {
        MMI_title_height = MMI_TITLE_HEIGHT;
        MMI_title_width = MMI_TITLE_WIDTH;
        MMI_title_x = MMI_TITLE_X;
        MMI_title_y = MMI_TITLE_Y;
    }
}   /* end of dm_reset_title_coordinates */


/*****************************************************************************
 * FUNCTION
 *  dm_reset_fixed_list_coordinates
 * DESCRIPTION
 *  Reset function for the fixed list coordinates so that other category screens get the original values only.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_fixed_list_coordinates(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_list_menu.x = MMI_content_x;
    MMI_fixed_list_menu.y = MMI_content_y;
    MMI_fixed_list_menu.width = MMI_content_width;
    MMI_fixed_list_menu.height = MMI_content_height;
}   /* end of dm_reset_fixed_list_coordinates */


/*****************************************************************************
 * FUNCTION
 *  dm_reset_inline_fixed_list_coordinates
 * DESCRIPTION
 *  Reset function for the inline fixed list coordinates so that other category screens get the original values only.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_inline_fixed_list_coordinates(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_list_menu.x = MMI_content_x;
    MMI_fixed_list_menu.y = MMI_content_y;
    MMI_fixed_list_menu.width = MMI_content_width;
    MMI_fixed_list_menu.height = MMI_content_height;
}   /* end of dm_reset_inline_fixed_list_coordinates */


/*****************************************************************************
 * FUNCTION
 *  dm_reset_lsk_coordinates
 * DESCRIPTION
 *  Reset function for the lsk coordinates so that other category screens get the original values only.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_lsk_coordinates(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_softkeys[MMI_LEFT_SOFTKEY].x = 0;
    MMI_softkeys[MMI_LEFT_SOFTKEY].y = UI_device_height - MMI_softkey_height;
    MMI_softkeys[MMI_LEFT_SOFTKEY].width = MMI_softkey_width;
    MMI_softkeys[MMI_LEFT_SOFTKEY].height = MMI_softkey_height;
}   /* end of dm_reset_lsk_coordinates */


/*****************************************************************************
 * FUNCTION
 *  dm_reset_rsk_coordinates
 * DESCRIPTION
 *  Reset function for the rsk coordinates so that other category screens get the original values only.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_rsk_coordinates(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_softkeys[MMI_RIGHT_SOFTKEY].x = UI_device_width - MMI_softkey_width;
    MMI_softkeys[MMI_RIGHT_SOFTKEY].y = UI_device_height - MMI_softkey_height;
    MMI_softkeys[MMI_RIGHT_SOFTKEY].width = MMI_softkey_width;
    MMI_softkeys[MMI_RIGHT_SOFTKEY].height = MMI_softkey_height;
}   /* end of dm_reset_rsk_coordinates */


/*****************************************************************************
 * FUNCTION
 *  dm_reset_multitaps_coordinates
 * DESCRIPTION
 *  Reset function for the multitap coordinates so that other category screens get the original values only.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_multitaps_coordinates(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_numberless_multitap();
    set_normal_multitap();
}   /* end of dm_reset_multitaps_coordinates */


/*****************************************************************************
 * FUNCTION
 *  dm_update_multitap_coordinates
 * DESCRIPTION
 *  This function updates the multitap coordinates depending on the multitap coordinates
 * PARAMETERS
 *  dm_multiline_inputbox_info      [IN]        Multiline inputbox information structure.
 *  dm_cat_scr_info                 [IN]        Category Screen information structure.
 * RETURNS
 *  void
 *****************************************************************************/
void dm_update_multitap_coordinates(
        dm_coordinates *dm_multiline_inputbox_info,
        dm_cat_scr_info_struct *dm_cat_scr_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 u8MultiTapCt = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Move the multitap depending on inputbox coordinates */
    for (u8MultiTapCt = 0; u8MultiTapCt < MAX_MULTITAPS; u8MultiTapCt++)
    {
        multitap_input *ami = NULL;

        switch (MMI_current_multitap_mode)
        {
            case 0:
                ami = &MMI_uppercase_multitaps[u8MultiTapCt];
                break;
            case 1:
                ami = &MMI_lowercase_multitaps[u8MultiTapCt];
                break;
                 
            case INPUT_MODE_MMI_MULTITAP_BPMF:
                ami = &MMI_bpmf_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_MULTITAP_PINYIN:
                ami = &MMI_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_ABC:
                ami = &MMI_lowercase_multitaps[u8MultiTapCt];
                /* CSD end */
        #if defined(__MMI_WCSS_INPUT_FORMAT_SUPPORT__)
            case INPUT_MODE_MULTITAP_UPPERCASE_ABC_NO_NUMERIC:
                ami = &MMI_uppercase_numberless_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_ABC_NO_NUMERIC:
                ami = &MMI_lowercase_numberless_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_WCSS_INPUT_FORMAT_SUPPORT__) */ 
                 
        #if defined(__MMI_MULTITAP_SPANISH__)
            case INPUT_MODE_MULTITAP_UPPERCASE_SPANISH:
                ami = &MMI_spanish_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_SPANISH:
                ami = &MMI_spanish_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_SPANISH:
                ami = &MMI_spanish_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_SPANISH__) */ 

    //qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
            case INPUT_MODE_MMI_MULTITAP_TAMIL:
                ami = &MMI_tamil_multitaps[u8MultiTapCt];
                break;
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
            case INPUT_MODE_MMI_MULTITAP_BENGALI:
                ami = &MMI_bengali_multitaps[u8MultiTapCt];
                break;
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
            case INPUT_MODE_MMI_MULTITAP_PUNJABI:
                ami = &MMI_punjabi_multitaps[u8MultiTapCt];
                break;
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
            case INPUT_MODE_MMI_MULTITAP_TELUGU:
                ami = &MMI_telugu_multitaps[u8MultiTapCt];
                break;
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
            case INPUT_MODE_MMI_MULTITAP_KANNADA:
                ami = &MMI_kannada_multitaps[u8MultiTapCt];
                break;
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
            case INPUT_MODE_MMI_MULTITAP_MALAYALAM:
                ami = &MMI_malayalam_multitaps[u8MultiTapCt];
                break;
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
            case INPUT_MODE_MMI_MULTITAP_ORIYA:
                ami = &MMI_oriya_multitaps[u8MultiTapCt];
                break;
#endif
//qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
            case INPUT_MODE_MMI_MULTITAP_MARATHI:
                ami = &MMI_marathi_multitaps[u8MultiTapCt];
                break;
#endif
#if defined(__MMI_MULTITAP_MARATHI__)
            case INPUT_MODE_MMI_MULTITAP_URDU:
                ami = &MMI_urdu_multitaps[u8MultiTapCt];
                break;
#endif
                 
        #if defined(__MMI_MULTITAP_POLISH__)
            case INPUT_MODE_MULTITAP_UPPERCASE_POLISH:
                ami = &MMI_polish_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_POLISH:
                ami = &MMI_polish_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_POLISH:
                ami = &MMI_polish_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_POLISH__) */ 
                 
                 
        #if defined(__MMI_MULTITAP_CZECH__)
            case INPUT_MODE_MULTITAP_UPPERCASE_CZECH:
                ami = &MMI_czech_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_CZECH:
                ami = &MMI_czech_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_CZECH:
                ami = &MMI_czech_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_CZECH__) */ 
                 
                 
        #if defined(__MMI_MULTITAP_TURKISH__)
            case INPUT_MODE_MULTITAP_UPPERCASE_TURKISH:
                ami = &MMI_turkish_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_TURKISH:
                ami = &MMI_turkish_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_TURKISH:
                ami = &MMI_turkish_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_TURKISH__) */ 
                 
        #if defined(__MMI_MULTITAP_FRENCH__)
            case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
                ami = &MMI_french_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
                ami = &MMI_french_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_FRENCH:
                ami = &MMI_french_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_FRENCH__) */ 
        #if defined(__MMI_MULTITAP_MYANMAR__)
            case INPUT_MODE_MULTITAP_MYANMAR:
                ami = &MMI_myanmar_multitaps[u8MultiTapCt];
                break;
        #endif
        #if defined(__MMI_MULTITAP_GERMAN__)
            case INPUT_MODE_MULTITAP_UPPERCASE_GERMAN:
                ami = &MMI_german_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_GERMAN:
                ami = &MMI_german_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_GERMAN:
                ami = &MMI_german_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_GERMAN__) */ 
        #if defined(__MMI_MULTITAP_ITALIAN__)
            case INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN:
                ami = &MMI_italian_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN:
                ami = &MMI_italian_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_ITALIAN:
                ami = &MMI_italian_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_ITALIAN__) */ 

                 

        #if defined(__MMI_MULTITAP_VIETNAMESE__)
            case INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE:
                ami = &MMI_vietnamese_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE:
                ami = &MMI_vietnamese_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_VIETNAMESE:
                ami = &MMI_vietnamese_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_VIETNAMESE__) */ 

                 

                 
        /*+ zhouqin modify for indonesian full keypad 20110419*/
        #if defined(__MMI_MULTITAP_INDONESIAN__) && !defined(__MMI_FULL_KEYPAD__)
        /*- zhouqin modify for indonesian full keypad 20110419*/
            case INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN:
                ami = &MMI_indonesian_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN:
                ami = &MMI_indonesian_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_INDONESIAN:
                ami = &MMI_indonesian_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_INDONESIAN__) */ 
                 
                //CSD end
                //CSD end
                 
        #if defined(__MMI_MULTITAP_DANISH__)
            case INPUT_MODE_MULTITAP_UPPERCASE_DANISH:
                ami = &MMI_danish_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_DANISH:
                ami = &MMI_danish_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_DANISH:
                ami = &MMI_danish_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_DANISH__) */ 
                 

                 
        #if defined(__MMI_MULTITAP_PORTUGUESE__)&& !defined(__MMI_FULL_KEYPAD__)
            case INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE:
                ami = &MMI_portuguese_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE:
                ami = &MMI_portuguese_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_PORTUGUESE:
                ami = &MMI_portuguese_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_PORTUGUESE__) */ 
                 
                 
        #if defined(__MMI_MULTITAP_FINNISH__)
            case INPUT_MODE_MULTITAP_UPPERCASE_FINNISH:
                ami = &MMI_finnish_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_FINNISH:
                ami = &MMI_finnish_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_FINNISH:
                ami = &MMI_finnish_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_FINNISH__) */ 
                 
 
        #if defined(__MMI_MULTITAP_SLOVAK__)
            case INPUT_MODE_MULTITAP_UPPERCASE_SLOVAK:
                ami = &MMI_slovak_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_SLOVAK:
                ami = &MMI_slovak_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_SLOVAK:
                ami = &MMI_slovak_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_SLOVAK__) */ 
        #if defined(__MMI_MULTITAP_DUTCH__)
            case INPUT_MODE_MULTITAP_UPPERCASE_DUTCH:
                ami = &MMI_dutch_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_DUTCH:
                ami = &MMI_dutch_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_DUTCH:
                ami = &MMI_dutch_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_DUTCH__) */ 
 
        #if defined(__MMI_MULTITAP_BULGARIAN__)
            case INPUT_MODE_MULTITAP_UPPERCASE_BULGARIAN:
                ami = &MMI_bulgarian_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_BULGARIAN:
                ami = &MMI_bulgarian_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_BULGARIAN:
                ami = &MMI_bulgarian_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_BULGARIAN__) */ 
 
        #if defined(__MMI_MULTITAP_ARABIC__)
            case INPUT_MODE_MULTITAP_ARABIC:
                ami = &MMI_arabic_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_ARABIC__) */ 
                /* Norwegian start */
        #if defined(__MMI_MULTITAP_NORWEGIAN__)
            case INPUT_MODE_MULTITAP_UPPERCASE_NORWEGIAN:
                ami = &MMI_norwegian_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_NORWEGIAN:
                ami = &MMI_norwegian_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_NORWEGIAN:
                ami = &MMI_norwegian_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_NORWEGIAN__) */ 
                //Norwegian end
 
                 
        #if defined(__MMI_MULTITAP_SWEDISH__)
            case INPUT_MODE_MULTITAP_UPPERCASE_SWEDISH:
                ami = &MMI_swedish_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_SWEDISH:
                ami = &MMI_swedish_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_SWEDISH:
                ami = &MMI_swedish_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_SWEDISH__) */ 
                 
                 
        #if defined(__MMI_MULTITAP_CROATIAN__)
            case INPUT_MODE_MULTITAP_UPPERCASE_CROATIAN:
                ami = &MMI_croatian_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_CROATIAN:
                ami = &MMI_croatian_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_CROATIAN:
                ami = &MMI_croatian_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_CROATIAN__) */ 
        #if defined(__MMI_MULTITAP_ROMANIAN__)
            case INPUT_MODE_MULTITAP_UPPERCASE_ROMANIAN:
                ami = &MMI_romanian_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_ROMANIAN:
                ami = &MMI_romanian_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_ROMANIAN:
                ami = &MMI_romanian_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_ROMANIAN__) */ 
        #if defined(__MMI_MULTITAP_SLOVENIAN__)
            case INPUT_MODE_MULTITAP_UPPERCASE_SLOVENIAN:
                ami = &MMI_slovenian_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_SLOVENIAN:
                ami = &MMI_slovenian_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_SLOVENIAN:
                ami = &MMI_slovenian_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_SLOVENIAN__) */ 
                 
                 
        #if defined(__MMI_MULTITAP_GREEK__)
            case INPUT_MODE_MULTITAP_UPPERCASE_GREEK:
                ami = &MMI_greek_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_GREEK:
                ami = &MMI_greek_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_GREEK:
                ami = &MMI_greek_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_GREEK__) */ 
        #if defined(__MMI_MULTITAP_HUNGARIAN__)
            case INPUT_MODE_MULTITAP_UPPERCASE_HUNGARIAN:
                ami = &MMI_hungarian_uppercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MULTITAP_LOWERCASE_HUNGARIAN:
                ami = &MMI_hungarian_lowercase_multitaps[u8MultiTapCt];
                break;
            case INPUT_MODE_MMI_LOWERCASE_HUNGARIAN:
                ami = &MMI_hungarian_lowercase_multitaps[u8MultiTapCt];
                break;
        #endif /* defined(__MMI_MULTITAP_HUNGARIAN__) */ 
                 
            default:
                PRINT_INFORMATION(("\nWARNING: Control should never come here."));

        }
        if (ami != NULL)
        {
            ami->x = dm_multiline_inputbox_info->S16X + 2;
            ami->y = dm_multiline_inputbox_info->S16Y + dm_multiline_inputbox_info->S16Height - MMI_multitap_height;    /* This should be with wrt to Multiline inputbox */
            /* ami->y = dm_cat_scr_info->S16Y1 + dm_multiline_inputbox_info->coordinate.S16Height;          //This is with respect to starting of the category screen which is 0 by default so dm_cat_scr_info.S32Y1 is added */
            ami->width = dm_multiline_inputbox_info->S16Width - 4;
            ami->height = MMI_multitap_height;
        }
    }
}   /* end of dm_update_multitap_coordinates */


/*****************************************************************************
 * FUNCTION
 *  dm_register_category_controlled_callback
 * DESCRIPTION
 *  Register the callback function for category controlled area
 * PARAMETERS
 *  f       [IN]        Callback function
 * RETURNS
 *  void
 *****************************************************************************/
void dm_register_category_controlled_callback(void (*f) (dm_coordinates *coordinate))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_category_controlled_area_callback = f;
}   /* end of dm_register_category_controlled_callback */


/*****************************************************************************
 * FUNCTION
 *  dm_register_category_controlled2_callback
 * DESCRIPTION
 *  Register the callback function for category controlled area2
 * PARAMETERS
 *  f       [IN]        Callback function
 * RETURNS
 *  void
 *****************************************************************************/
void dm_register_category_controlled2_callback(void (*f) (dm_coordinates *coordinate))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_category_controlled_area2_callback = f;
}   /* end of dm_register_category_controlled_callback */


/*****************************************************************************
 * FUNCTION
 *  dm_reset_category_controlled_callback
 * DESCRIPTION
 *  Reset the callback function for category controlled area.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_category_controlled_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_category_controlled_area_callback = NULL;
    dm_category_controlled_area2_callback = NULL;
}   /* end of dm_reset_category_controlled_callback */

#ifdef __MMI_SUBLCD__


/*****************************************************************************
 * FUNCTION
 *  dm_register_sub_category_controlled_callback
 * DESCRIPTION
 *  Register the callback function for category controlled area
 * PARAMETERS
 *  f       [IN]        Callback function
 * RETURNS
 *  void
 *****************************************************************************/
void dm_register_sub_category_controlled_callback(void (*f) (dm_coordinates *coordinate))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_sub_category_controlled_area_callback = f;
}   /* end of dm_register_sub_category_controlled_callback */


/*****************************************************************************
 * FUNCTION
 *  dm_reset_sub_category_controlled_callback
 * DESCRIPTION
 *  Reset the callback function for category controlled area.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_sub_category_controlled_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_sub_category_controlled_area_callback = NULL;
}   /* end of dm_reset_sub_category_controlled_callback */

#endif /* __MMI_SUBLCD__ */ 


/*****************************************************************************
 * FUNCTION
 *  dm_reset_str_image
 * DESCRIPTION
 *  This function ius used to reset the image and string areas count to 0.
 * PARAMETERS
 *  void
 *  UICtrlAccessPtr_p(?)        [IN/OUT]        It specifies the start position to take out the coordinates.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_str_image(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_string_count = 0;
    dm_image_count = 0;
    dm_back_fill_count = 0;
    dm_rectangle_count = 0;
    dm_line_count = 0;
}

#if (0)
#endif /* (0) */ 


/*****************************************************************************
 * FUNCTION
 *  dm_add_image
 * DESCRIPTION
 *  This function is used to add image data. To be called by Category Screen
 * PARAMETERS
 *  img                         [IN]        
 *  image_name                  [IN]        
 *  background                  [IN]        Background Fill for the image
 *  transparent_color(?)        [IN]        Transparent Color for the Image
 *  image(?)                    [IN]        Image to be displayed.
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
void dm_add_image(PU8 img, UI_string_type image_name, UI_filled_area *background)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    GDI_HANDLE act_lcd_handle;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* img & image_name can't be NULL at the same time */
    /* MMI_ASSERT ( img!=NULL || image_name!=NULL ); */

    gdi_lcd_get_active(&act_lcd_handle);

#ifdef __MMI_SUBLCD__
    if (act_lcd_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        MMI_ASSERT(dm_image_count_sub < DM_MAX_SUBLCD_IMAGES);

        dm_image_array_sub[dm_image_count_sub].back_fill = background;
        dm_image_array_sub[dm_image_count_sub].image = (PU8) img;
        dm_image_array_sub[dm_image_count_sub].name = image_name;
        dm_image_array_sub[dm_image_count_sub++].image_handle = GDI_ERROR_HANDLE;

    }
    else
#endif /* __MMI_SUBLCD__ */ 
    {
        MMI_ASSERT(dm_image_count < DM_MAX_IMAGES);

        dm_image_array[dm_image_count].back_fill = background;
        dm_image_array[dm_image_count].image = (PU8) img;
        dm_image_array[dm_image_count].name = image_name;
        dm_image_array[dm_image_count++].image_handle = GDI_ERROR_HANDLE;
    }
}

/*****************************************************************************
 * FUNCTION
 *  dm_get_image_handle
 * DESCRIPTION
 *  This function is used to get the animation handle. To be called by Category Screen
 * PARAMETERS
 *  count                       [IN]        the nummer of the animation
 * RETURNS
 *  gdi_handle
 *****************************************************************************/
gdi_handle dm_get_image_handle(S32 count) 
{
    return dm_image_array[count].image_handle;
}


/*****************************************************************************
 * FUNCTION
 *  dm_change_image
 * DESCRIPTION
 *  This function is used to change image data. To be called by Category Screen
 * PARAMETERS
 *  image       [IN]        Image to be changed.
 *  name        [IN]        Image Name to be changed
 *  index       [IN]        Index in dm_image_struct
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
void dm_change_image(PU8 image, UI_string_type name, S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Only image or name should be changed at one time */
    //      MMI_ASSERT ( ( name==NULL || image==NULL ) && ( name!= NULL || image!=NULL ) );
    //      if ( ( name!=NULL && image!=NULL ) || ( name== NULL && image==NULL ) )
    //              MMI_ASSERT(0);//Only one of two should be assigned.

    /* undefined image */
    MMI_ASSERT(index < dm_image_count);

    dm_image_array[index].image = image;
    dm_image_array[index].name = name;
}


/*****************************************************************************
 * FUNCTION
 *  dm_add_string
 * DESCRIPTION
 *  This function is used to add string data. To be called by Category Screen
 * PARAMETERS
 *  string              [IN]        String to be displayed.
 *  font                [IN]        Font to be used to draw the string
 *  text_color          [IN]        Text Color for the string
 *  border_color        [IN]        Border Color for the string
 *  background          [IN]        Background Fill for the string
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
void dm_add_string(
        UI_string_type string,
        UI_font_type font,
        color_t text_color,
        color_t border_color,
        UI_filled_area *background)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    GDI_HANDLE act_lcd_handle;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_lcd_get_active(&act_lcd_handle);

#ifdef __MMI_SUBLCD__
    if (act_lcd_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        MMI_ASSERT(dm_string_count_sub < DM_MAX_SUBLCD_STRINGS);

        dm_string_array_sub[dm_string_count_sub].text_color = text_color;
        dm_string_array_sub[dm_string_count_sub].border_color = border_color;
        dm_string_array_sub[dm_string_count_sub].font = font;
        if (background)
        {
            dm_string_array_sub[dm_string_count_sub].back_fill = *background;
        }
        dm_string_array_sub[dm_string_count_sub++].string = string;
    }
    else
#endif /* __MMI_SUBLCD__ */ 
    {
        MMI_ASSERT(dm_string_count < DM_MAX_STRINGS);

        dm_string_array[dm_string_count].text_color = text_color;
        dm_string_array[dm_string_count].border_color = border_color;
        dm_string_array[dm_string_count].font = font;
        if (background)
        {
            dm_string_array[dm_string_count].back_fill = *background;
        }
        dm_string_array[dm_string_count++].string = string;
    }
}


/*****************************************************************************
 * FUNCTION
 *  dm_reset_button_bar_coordinates
 * DESCRIPTION
 *  This function resets button bar to its original height.
 *  Reuired till all categories are moved to Draw Manager
 * PARAMETERS
 *  void
 *  UICtrlAccessPtr_p(?)        [IN/OUT]        It specifies the start position to take out the coordinates.
 *  dm_image_info_struct I  N/OUT image area coordinate info.(?)
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_button_bar_coordinates(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    set_button_bar_height(MMI_BUTTON_BAR_HEIGHT);

    /* Right softkey always have UI_BUTTON_RIGHT_ALIGN */
    set_softkey_flags_off(UI_BUTTON_RIGHT_ALIGN, MMI_LEFT_SOFTKEY);
    set_softkey_flags_on(UI_BUTTON_LEFT_ALIGN, MMI_LEFT_SOFTKEY);

#ifdef __MMI_WGUI_CSK_ENABLE__
    set_softkey_flags_off(UI_BUTTON_RIGHT_ALIGN, MMI_CENTER_SOFTKEY);
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  dm_add_back_fill_area
 * DESCRIPTION
 *  This function is used to add back fill. To be called by Category Screen
 * PARAMETERS
 *  background      [IN]        Background Fill for the area
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
void dm_add_back_fill_area(UI_filled_area *background)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (dm_back_fill_count >= DM_MAX_FILL_AREAS)
    {
        return;
    }
    dm_back_fill_array[dm_back_fill_count++] = background;
}


/*****************************************************************************
 * FUNCTION
 *  dm_add_rectangle
 * DESCRIPTION
 *  This function is used to add rectangle. To be called by Category Screen
 * PARAMETERS
 *  line_color          [IN]        Color for the reactangle
 *  fill_rectangle      [IN]        In Whether the rectangle needs to be filled from inside
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
void dm_add_rectangle(color_t line_color, dm_rectangle_fill_style fill_rectangle)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (dm_rectangle_count >= DM_MAX_RECTANGLES)
    {
        return;
    }
    dm_rectangle_array[dm_rectangle_count].fill_rectangle = fill_rectangle;
    dm_rectangle_array[dm_rectangle_count++].line_color = line_color;
}


/*****************************************************************************
 * FUNCTION
 *  dm_add_line
 * DESCRIPTION
 *  This function is used to add line. To be called by Category Screen
 * PARAMETERS
 *  line_color      [IN]        Color for the reactangle
 *  *(?)
 * RETURNS
 *  void
 *****************************************************************************/
void dm_add_line(color_t line_color)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (dm_line_count >= DM_MAX_LINES)
    {
        return;
    }
    dm_line_array[dm_line_count++] = line_color;
}


/*****************************************************************************
 * FUNCTION
 *  dm_add_slide_control
 * DESCRIPTION
 *  This function is used to add slide control. To be called by Category Screen
 * PARAMETERS
 *  lower_limit         [IN]        
 *  upper_limit         [IN]        
 *  current_value       [IN]        
 *  change_callback     [IN]        
 *  label_string        [?]         
 *  line_color(?)       [IN]        Color for the reactangle
 * RETURNS
 *  void
 *****************************************************************************/
void dm_add_slide_control(
        S32 lower_limit,
        S32 upper_limit,
        S32 current_value,
        void (*change_callback) (S32 value),
        U8 *label_string)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (dm_slide_control_count >= MMI_MAX_SLIDE_CONTROL)
    {
        MMI_ASSERT(dm_slide_control_count < MMI_MAX_SLIDE_CONTROL);
        return;
    }
    wgui_setup_slide_control(
        &MMI_slide_control[dm_slide_control_count],
        0,
        0,
        lower_limit,
        upper_limit,
        current_value,
        change_callback);
    set_slide_control_label(&MMI_slide_control[dm_slide_control_count++], label_string);
}


/*****************************************************************************
 * FUNCTION
 *  dm_get_slide_control_count
 * DESCRIPTION
 *  This function is used to get count of active slide controls. To be called by Category Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  S32
 *****************************************************************************/
S32 dm_get_slide_control_count(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return dm_slide_control_count;
}


/*****************************************************************************
 * FUNCTION
 *  dm_scroll_text_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_scroll_text_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_handle_scrolling_text(dm_current_scroll_text);
}


/*****************************************************************************
 * FUNCTION
 *  dm_add_scroll_text
 * DESCRIPTION
 *  This function is used to add scroll text. To be called by Category Screen
 * PARAMETERS
 *  text                [IN]        
 *  timer_callback      [IN]        
 *  draw_background     [IN]        
 *  text_color          [IN]        
 *  border_color        [IN]        
 *  line_color(?)       [IN]        Color for the reactangle
 * RETURNS
 *  void
 *****************************************************************************/
extern UI_string_type UI_strcpy(UI_string_type text1, UI_string_type text2);
void dm_add_scroll_text(
        UI_string_type text,
        void (*timer_callback) (void),
        void (*draw_background) (S32 x1, S32 y1, S32 x2, S32 y2),
        color_t text_color,
        color_t border_color)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 string_width = 0, string_height = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (dm_scroll_text_count >= DM_MAX_SCROLL_TEXT)
    {
        MMI_ASSERT(dm_scroll_text_count < DM_MAX_SCROLL_TEXT);
        return;
    }
    if (text == NULL)
    {
        dm_scroll_text_buf[dm_scroll_text_count][0] = 0;
        dm_scroll_text_buf[dm_scroll_text_count][1] = 0;
    }
    else
    {
        UI_strcpy((UI_string_type) dm_scroll_text_buf[dm_scroll_text_count], text);
        dm_current_scroll_text = &dm_scroll_text[dm_scroll_text_count];
        gui_measure_string(text, &string_width, &string_height);
        gui_create_scrolling_text(
            dm_current_scroll_text,
            20,
            20,
            string_width,
            string_height,
            (UI_string_type) dm_scroll_text_buf[dm_scroll_text_count++],
            dm_scroll_text_handler,
            draw_background,
            text_color,
            border_color);
        if (text_color.r != border_color.r || text_color.g != border_color.g || text_color.b != border_color.b)
        {
            dm_current_scroll_text->flags |= UI_SCROLLING_TEXT_BORDERED_TEXT;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  dm_add_button
 * DESCRIPTION
 *  This function is used to add icon button. To be called by Category Screen
 * PARAMETERS
 *  text            [IN]        
 *  up_image        [IN]        Button up image
 *  down_image      [IN]        Button down image
 *  clear_fp        [IN]        
 * RETURNS
 *  U16         index
 *****************************************************************************/
U16 dm_add_button(UI_string_type text, PU8 up_image, PU8 down_image, gui_button_clear_background_hdlr clear_fp)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 curr_idx = dm_button_count++;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TRACE_GDI_FUNCTION();

    mmi_trace(g_sw_GDI,"up_image = 0x%x", up_image);
	
    gui_create_icontext_button(&dm_button_array[curr_idx], -1, -1, -1, -1, text, up_image);
    dm_button_array[curr_idx].flags |= UI_BUTTON_DISABLE_BACKGND_DISPLAY | UI_BUTTON_SHIFT_BUTTON1;
    if (text != NULL)
    {
        dm_button_array[curr_idx].flags |= UI_BUTTON_CENTER_TEXT_X | UI_BUTTON_CENTER_TEXT_Y;
    }
    if (up_image != NULL || down_image != NULL)
    {
        dm_button_array[curr_idx].flags |= UI_BUTTON_CENTER_ICON_X | UI_BUTTON_CENTER_ICON_Y;
    }
    dm_button_array[curr_idx].clicked_down_icon = down_image;
    dm_button_array[curr_idx].clear_background_fp = clear_fp;

    return curr_idx;
}


/*****************************************************************************
 * FUNCTION
 *  dm_move_button
 * DESCRIPTION
 *  This function is used to move icon button. To be called by Category Screen
 * PARAMETERS
 *  index       [IN]        Index of dm_button_array
 *  x           [IN]        X coordinate
 *  y           [IN]        Y coordinate
 * RETURNS
 *  void
 *****************************************************************************/
void dm_move_button(U16 index, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_move_icontext_button(&dm_button_array[dm_button_count], x, y);
}


/*****************************************************************************
 * FUNCTION
 *  dm_resize_button
 * DESCRIPTION
 *  This function is used to move icon button. To be called by Category Screen
 * PARAMETERS
 *  index       [IN]        Index of dm_button_array
 *  width       [IN]        Width of icon button
 *  height      [IN]        Height of icon button
 * RETURNS
 *  void
 *****************************************************************************/
void dm_resize_button(U16 index, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_resize_icontext_button(&dm_button_array[dm_button_count], width, height);
    dm_button_array[dm_button_count].icon_x = 0;
    dm_button_array[dm_button_count].icon_y = 0;
}


/*****************************************************************************
 * FUNCTION
 *  dm_redraw_button
 * DESCRIPTION
 *  This function is used to redraw icon button. To be called by Category Screen
 * PARAMETERS
 *  index       [IN]        Index of dm_button_array
 * RETURNS
 *  void
 *****************************************************************************/
void dm_redraw_button(U16 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_show_icontext_button(&dm_button_array[dm_button_count]);
}


/*****************************************************************************
 * FUNCTION
 *  dm_get_control_coordinates_from_category
 * DESCRIPTION
 *  This function is used obtain the line coordinates.
 * PARAMETERS
 *  S32ScrId                    [IN]            
 *  S32CatId                    [IN]            
 *  control                     [IN]            
 *  index                       [IN]            
 *  coordinate                  [?]             Info.
 *  UICtrlAccessPtr_p(?)        [IN/OUT]        It specifies the start position to take out the coordinates.
 * RETURNS
 *  void
 *****************************************************************************/
void dm_get_control_coordinates_from_category(
        S32 S32ScrId,
        S32 S32CatId,
        mmi_dm_control_ids_enum control,
        S32 index,
        dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 u8CtrlCt = 0;
    S32 u8NoOfUICtrls = 0;
    S16 *UICtrlAccessPtr_p = NULL, *DeafultCoordinateSet_p = NULL;
    typedef struct
    {
        dm_coordinates coordinates;
        S16 array[100];
    } temp_coordinates_struct;
    temp_coordinates_struct temp_coordinates;
    S32 idx = index;
    U8 *control_set_ptr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    control_set_ptr = dm_search_control_set(S32CatId, &DeafultCoordinateSet_p);

    /* First element in Category-Controls map specifies the number of controls */
    u8NoOfUICtrls = control_set_ptr[0];

    if (S32ScrId >= 0)
    {
        UICtrlAccessPtr_p = dm_search_coordinate_set(S32ScrId);
    }
    if (UICtrlAccessPtr_p == NULL)
    {
        UICtrlAccessPtr_p = DeafultCoordinateSet_p;
    }
    /* Fist element in coordinate set specifies the coordinates of complete category screen */
    UICtrlAccessPtr_p = dm_get_cat_scr_coordinates(UICtrlAccessPtr_p, (dm_cat_scr_info_struct*)(void *)&temp_coordinates);

    for (u8CtrlCt = 1; u8CtrlCt <= u8NoOfUICtrls; u8CtrlCt++)
    {
        switch (control_set_ptr[u8CtrlCt])
        {
            case DM_BASE_LAYER_START:
            case DM_NEW_LAYER_END:
            case DM_BASE_CONTROL_SET1:
            case DM_ALIGNED_AREA_END:
                break;

            case DM_NEW_LAYER_START:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_CIRCULAR_MENU1:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_LIST1:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_DYNAMIC_LIST1:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_MATRIX_MENU1:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_MULTILINE_INPUTBOX1:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_DIALER_INPUT_BOX1:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_SINGLELINE_INPUTBOX1:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_LSK:
            case DM_RSK:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_TITLE1:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_CATEGORY_CONTROLLED_AREA:
            case DM_CATEGORY_CONTROLLED_AREA2:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_STATUS_BAR1:
            {
                UICtrlAccessPtr_p = dm_get_status_icon_bar_coordinates(UICtrlAccessPtr_p, (dm_status_info_struct*)(void *)&temp_coordinates);
                break;
            }
            case DM_BUTTON_BAR1:
            {
                UICtrlAccessPtr_p = dm_get_button_bar_coordinates(UICtrlAccessPtr_p, (dm_buttonbar_info_struct*)(void *)&temp_coordinates);
                break;
            }
            case DM_INLINE_FIXED_LIST1:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_ALIGNED_AREA_START:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_STRING:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                if (control == control_set_ptr[u8CtrlCt])
                {
                    idx--;
                }
                break;
            }
            case DM_IMAGE:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                if (control == control_set_ptr[u8CtrlCt])
                {
                    idx--;
                }
                break;
            }
            case DM_CALENDAR:
            {
                UICtrlAccessPtr_p = dm_get_calendar_coordinates(UICtrlAccessPtr_p, (dm_calendar_info_struct*)(void *)&temp_coordinates);
                break;
            }
            case DM_BACK_FILL_AREA:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                if (control == control_set_ptr[u8CtrlCt])
                {
                    idx--;
                }
                break;
            }
            case DM_RECTANGLE:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                if (control == control_set_ptr[u8CtrlCt])
                {
                    idx--;
                }
                break;
            }
            case DM_LINE:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                if (control == control_set_ptr[u8CtrlCt])
                {
                    idx--;
                }
                break;
            }
            case DM_SLIDE_CONTROL:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                if (control == control_set_ptr[u8CtrlCt])
                {
                    idx--;
                }
                break;
            }
            case DM_TYPICAL_STOPWATCH:
            {
                UICtrlAccessPtr_p = dm_get_coordinates(UICtrlAccessPtr_p, (dm_coordinates*) & temp_coordinates);
                break;
            }
            case DM_NWAY_STOPWATCH:
            {
                UICtrlAccessPtr_p = dm_get_typical_stopwatch_coordinates(
                                        UICtrlAccessPtr_p,
                                        (dm_typical_stopwatch_info_struct*)(void *)&temp_coordinates);
                break;
            }
            case DM_POPUP_BACKGROUND:
            {
                UICtrlAccessPtr_p = dm_get_nway_stopwatch_coordinates(
                                        UICtrlAccessPtr_p,
                                        (dm_nway_stopwatch_info_struct*) (void *)&temp_coordinates);
                break;
            }

        }

        if (idx == -1 && control == control_set_ptr[u8CtrlCt])
        {
            *coordinate = temp_coordinates.coordinates;
            return;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  dm_create_layer
 * DESCRIPTION
 *  This function is to create new layer which is not defined in control_set
 * PARAMETERS
 *  x               [IN]        X-coordinate of new layer
 *  y               [IN]        Y-coordinate of new layer
 *  width           [IN]        Width of new layer
 *  height          [IN]        Height of new layer
 *  handle_ptr      [?]         
 *  flags           [IN]        Flags to identify the index of new layer
 * RETURNS
 *  GDI_RESULT  result of create layer
 *****************************************************************************/
GDI_RESULT dm_create_layer(S32 x, S32 y, S32 width, S32 height, gdi_handle *handle_ptr, U32 flags)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 idx = 0, act_layer_idx = 0, *act_dm_layers_count = NULL;
    U16 i = 0; //061506 double buffer
    GDI_HANDLE act_layer = GDI_ERROR_HANDLE, *act_dm_layers = NULL;
    GDI_RESULT ret = 0;
    gdi_handle act_lcd_handle;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_lcd_get_active(&act_lcd_handle);

#ifdef __MMI_SUBLCD__
    if (act_lcd_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        act_dm_layers = dm_layers_sub;
        act_dm_layers_count = &dm_layers_count_sub;
    }
    else if (act_lcd_handle == GDI_LCD_MAIN_LCD_HANDLE)
#endif /* __MMI_SUBLCD__ */ 
    {
        act_dm_layers = dm_layers;
        act_dm_layers_count = &dm_layers_count;
    }

    switch (flags)
    {
        case DM_LAYER_BOTTOM:

            /* move each layer forward */
            /* 061506 double buffer Start */
            for (idx = *act_dm_layers_count; idx > 0; idx--)
            {
                act_dm_layers[idx] = act_dm_layers[idx-1];
            }
            /* 061506 double buffer End */

            /* create new layer using GDI APIs */
            ret = gdi_layer_create(x, y, width, height, &act_dm_layers[0]);

            *handle_ptr = act_dm_layers[0];

            (*act_dm_layers_count)++;

            break;

        case DM_LAYER_TOP:

            /* create new layer using GDI APIs */
            ret = gdi_layer_create(x, y, width, height, &act_dm_layers[*act_dm_layers_count]);

            *handle_ptr = act_dm_layers[(*act_dm_layers_count)];

            (*act_dm_layers_count)++;

            break;

        case DM_LAYER_ACTIVE_TOP:

            /* get active layer handle */
            gdi_layer_get_active(&act_layer);

            /* find out active layer index in dm_layers */
            for (idx = 0; idx < *act_dm_layers_count; idx++)
                if (act_dm_layers[idx] == act_layer)
                {
                    break;
                }

            act_layer_idx = idx;

            /* move each layer forward */
            /* 061506 double buffer Start */
            for (i = *act_dm_layers_count; i > idx; i--)
            {
                act_dm_layers[i] = act_dm_layers[i-1];
            }
            /* 061506 double buffer End */

            (*act_dm_layers_count)++;

            /* create new layer using GDI APIs */
            ret = gdi_layer_create(x, y, width, height, &act_dm_layers[act_layer_idx]);

            *handle_ptr = act_dm_layers[act_layer_idx];

            break;

        case DM_LAYER_ACTIVE_BOTTOM:

            if (*act_dm_layers_count == 0)
            {
                act_layer_idx = 0;
                ret = gdi_layer_create(x, y, width, height, &act_dm_layers[act_layer_idx]);
                *act_dm_layers_count = 1;
                *handle_ptr = act_dm_layers[act_layer_idx];
                break;
            }
            /* get active layer handle */
            gdi_layer_get_active(&act_layer);

            /* find out active layer index in dm_layers */
            for (idx = 0; idx < *act_dm_layers_count; idx++)
                if (act_dm_layers[idx] == act_layer)
                {
                    break;
                }

            act_layer_idx = ++idx;

            /* move each layer forward */
            /* 061506 double buffer Start */
            for (i = *act_dm_layers_count; i > idx; i--)
            {
                act_dm_layers[i] = act_dm_layers[i-1];
            }
            /* 061506 double buffer End */

            (*act_dm_layers_count)++;

            /* create new layer using GDI APIs */
            ret = gdi_layer_create(x, y, width, height, &act_dm_layers[act_layer_idx]);

            *handle_ptr = act_dm_layers[act_layer_idx];

            break;
    }

    gdi_layer_set_blt_layer(act_dm_layers[0], act_dm_layers[1], act_dm_layers[2], act_dm_layers[3]);

    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  dm_create_layer_using_outside_memory
 * DESCRIPTION
 *  This function is to create new layer which is not defined in control_set and use outside memory
 * PARAMETERS
 *  x                       [IN]        X-coordinate of new layer
 *  y                       [IN]        Y-coordinate of new layer
 *  width                   [IN]        Width of new layer
 *  height                  [IN]        Height of new layer
 *  handle_ptr              [IN]        
 *  outside_memory          [IN]        Outside memory pointer
 *  outside_memory_size     [IN]        Outside memory size
 *  flags                   [IN]        Flags to identify the index of new layer
 * RETURNS
 *  void
 *****************************************************************************/
GDI_RESULT dm_create_layer_using_outside_memory(
            S32 x,
            S32 y,
            S32 width,
            S32 height,
            gdi_handle **handle_ptr,
            U8 *outside_memory,
            S32 outside_memory_size,
            U32 flags)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 idx = 0, act_layer_idx = 0, *act_dm_layers_count = NULL;
    U16 i = 0; //061506 double buffer
    GDI_HANDLE act_layer = GDI_ERROR_HANDLE, *act_dm_layers = NULL;
    GDI_RESULT ret = 0;
    gdi_handle act_lcd_handle;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_lcd_get_active(&act_lcd_handle);

#ifdef __MMI_SUBLCD__
    if (act_lcd_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        act_dm_layers = dm_layers_sub;
        act_dm_layers_count = &dm_layers_count_sub;
    }
    else if (act_lcd_handle == GDI_LCD_MAIN_LCD_HANDLE)
#endif /* __MMI_SUBLCD__ */ 
    {
        act_dm_layers = dm_layers;
        act_dm_layers_count = &dm_layers_count;
    }

    switch (flags)
    {
        case DM_LAYER_BOTTOM:

            /* move each layer forward */
            /* 061506 double buffer Start */
            for (idx = *act_dm_layers_count; idx > 0; idx--)
            {
                act_dm_layers[idx] = act_dm_layers[idx-1];
            }
            /* 061506 double buffer End */

            /* create new layer using GDI APIs */
            ret = gdi_layer_create_using_outside_memory(
                    x,
                    y,
                    width,
                    height,
                    &act_dm_layers[0],
                    outside_memory,
                    outside_memory_size);

            (*act_dm_layers_count)++;

            *handle_ptr = &act_dm_layers[0];

            break;

        case DM_LAYER_TOP:

            /* create new layer using GDI APIs */
            ret = gdi_layer_create_using_outside_memory(
                    x,
                    y,
                    width,
                    height,
                    &act_dm_layers[*act_dm_layers_count],
                    outside_memory,
                    outside_memory_size);

            *handle_ptr = &act_dm_layers[*act_dm_layers_count];

            (*act_dm_layers_count)++;

            break;

        case DM_LAYER_ACTIVE_TOP:

            /* get active layer handle */
            gdi_layer_get_active(&act_layer);

            /* find out active layer index in dm_layers */
            for (idx = 0; idx < *act_dm_layers_count; idx++)
                if (act_dm_layers[idx] == act_layer)
                {
                    break;
                }

            act_layer_idx = idx;

            /* move each layer forward */
            /* 061506 double buffer Start */
            for (i = *act_dm_layers_count; i > idx; i--)
            {
                act_dm_layers[i] = act_dm_layers[i-1];
            }
            /* 061506 double buffer End */

            (*act_dm_layers_count)++;

            /* create new layer using GDI APIs */
            ret = gdi_layer_create_using_outside_memory(
                    x,
                    y,
                    width,
                    height,
                    &act_dm_layers[act_layer_idx],
                    outside_memory,
                    outside_memory_size);

            *handle_ptr = &act_dm_layers[act_layer_idx];

            break;

        case DM_LAYER_ACTIVE_BOTTOM:

            /* get active layer handle */
            gdi_layer_get_active(&act_layer);

            /* find out active layer index in dm_layers */
            for (idx = 0; idx < *act_dm_layers_count; idx++)
                if (act_dm_layers[idx] == act_layer)
                {
                    break;
                }

            act_layer_idx = ++idx;

            /* move each layer forward */
            /* 061506 double buffer Start */
            for (i = *act_dm_layers_count; i > idx; i--)
            {
                act_dm_layers[i] = act_dm_layers[i-1];
            }
            /* 061506 double buffer End */

            (*act_dm_layers_count)++;

            /* create new layer using GDI APIs */
            ret = gdi_layer_create_using_outside_memory(
                    x,
                    y,
                    width,
                    height,
                    &act_dm_layers[act_layer_idx],
                    outside_memory,
                    outside_memory_size);

            *handle_ptr = &act_dm_layers[act_layer_idx];

            break;
    }

    gdi_layer_set_blt_layer(act_dm_layers[0], act_dm_layers[1], act_dm_layers[2], act_dm_layers[3]);

    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  dm_add_blt_layer
 * DESCRIPTION
 *  This function is to add new layer into blt layer list
 * PARAMETERS
 *  handle_ptr      [IN]        New layer handle
 *  flags           [IN]        Flags to identify the index of new layer
 * RETURNS
 *  MMI_BOOL result of adding layer
 *****************************************************************************/
MMI_BOOL dm_add_blt_layer(gdi_handle handle_ptr, U32 flags)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 idx = 0, act_layer_idx = 0, *act_dm_layers_count = NULL;
    U16 i = 0; //061506 double buffer
    GDI_HANDLE act_layer = GDI_ERROR_HANDLE, *act_dm_layers = NULL;
    gdi_handle act_lcd_handle;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_lcd_get_active(&act_lcd_handle);

#ifdef __MMI_SUBLCD__
    if (act_lcd_handle == GDI_LCD_SUB_LCD_HANDLE)
    {
        act_dm_layers = dm_layers_sub;
        act_dm_layers_count = &dm_layers_count_sub;
    }
    else if (act_lcd_handle == GDI_LCD_MAIN_LCD_HANDLE)
#endif /* __MMI_SUBLCD__ */ 
    {
        act_dm_layers = dm_layers;
        act_dm_layers_count = &dm_layers_count;
    }

    for (idx = 0; idx < *act_dm_layers_count; idx++)
    {
        if (act_dm_layers[idx] == handle_ptr)
        {
            return MMI_FALSE;
        }
    }

    switch (flags)
    {
        case DM_LAYER_BOTTOM:

            /* move each layer forward */
            /* 061506 double buffer Start */
            for (idx = *act_dm_layers_count; idx > 0; idx--)
            {
                act_dm_layers[idx] = act_dm_layers[idx-1];
            }
            /* 061506 double buffer End */

            act_dm_layers[0] = handle_ptr;

            (*act_dm_layers_count)++;

            break;

        case DM_LAYER_TOP:

            act_dm_layers[(*act_dm_layers_count)] = handle_ptr;

            (*act_dm_layers_count)++;

            break;

        case DM_LAYER_ACTIVE_TOP:

            /* get active layer handle */
            gdi_layer_get_active(&act_layer);

            /* find out active layer index in dm_layers */
            for (idx = 0; idx < *act_dm_layers_count; idx++)
                if (act_dm_layers[idx] == act_layer)
                {
                    break;
                }

            act_layer_idx = idx;

            /* move each layer forward */
            /* 061506 double buffer Start */
            for (i = *act_dm_layers_count; i > idx; i--)
            {
                act_dm_layers[i] = act_dm_layers[i-1];
            }
            /* 061506 double buffer End */

            (*act_dm_layers_count)++;

            act_dm_layers[act_layer_idx] = handle_ptr;

            break;

        case DM_LAYER_ACTIVE_BOTTOM:

            /* get active layer handle */
            gdi_layer_get_active(&act_layer);

            /* find out active layer index in dm_layers */
            for (idx = 0; idx < *act_dm_layers_count; idx++)
                if (act_dm_layers[idx] == act_layer)
                {
                    break;
                }

            act_layer_idx = ++idx;

            /* move each layer forward */
            /* 061506 double buffer Start */
            for (i = *act_dm_layers_count; i > idx; i--)
            {
                act_dm_layers[i] = act_dm_layers[i-1];
            }
            /* 061506 double buffer End */

            (*act_dm_layers_count)++;

            act_dm_layers[act_layer_idx] = handle_ptr;

            break;
    }

    gdi_layer_set_blt_layer(act_dm_layers[0], act_dm_layers[1], act_dm_layers[2], act_dm_layers[3]);

    return MMI_TRUE;

}


/*****************************************************************************
 * FUNCTION
 *  dm_set_blt_layer
 * DESCRIPTION
 *  This function is to set blt layer sequence
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
GDI_RESULT dm_set_blt_layer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_layer_set_blt_layer(dm_layers[0], dm_layers[1], dm_layers[2], dm_layers[3]);
}

 


/*****************************************************************************
 * FUNCTION
 *  dm_register_button_functions
 * DESCRIPTION
 *  set handling functions of buttons
 * PARAMETERS
 *  button_index        [IN]        Index of dm_button_array
 *  key_type            [IN]        Key event type
 *  f                   [IN]        Handling function pointer
 * RETURNS
 *  void
 *****************************************************************************/
void dm_register_button_functions(U16 button_index, U16 key_type, FuncPtr f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(button_index < DM_MAX_BUTTONS);

    dm_button_func_ptrs[button_index][key_type] = f;
}


/*****************************************************************************
 * FUNCTION
 *  dm_reset_button_functions
 * DESCRIPTION
 *  reset handling functions of buttons
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dm_reset_button_functions(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 button_index, key_type = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (button_index = 0; button_index < DM_MAX_BUTTONS; button_index++)
        for (key_type = 0; key_type < MAX_KEY_TYPE; key_type++)
        {
            dm_button_func_ptrs[button_index][key_type] = NULL;
        }
}


/*****************************************************************************
 * FUNCTION
 *  dm_execute_button_handler
 * DESCRIPTION
 *  Execute left softkey handler from pen
 * PARAMETERS
 *  btn_index       [IN]        
 *  key_type        [IN]        - KEY_EVENT_DOWN, KEY_EVENT_UP, KEY_LONG_PRESS
 * RETURNS
 *  void
 *****************************************************************************/
void dm_execute_button_handler(U16 btn_index, U16 key_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(btn_index < DM_MAX_BUTTONS);
    MMI_ASSERT(key_type < MAX_KEY_TYPE);

    if (dm_button_func_ptrs[btn_index][key_type] != NULL)
    {
        dm_button_func_ptrs[btn_index][key_type] ();
    }
}


/*****************************************************************************
 * FUNCTION
 *  dm_is_redraw_screen
 * DESCRIPTION
 *  Execute left softkey handler from pen
 * PARAMETERS
 *  void
 *  key_type - KEY_EVENT_DOWN, KEY_EVENT_UP, KEY_LONG_PRESS(?)
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL dm_is_redraw_screen(void)
{
#ifdef __MMI_SUBLCD__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_dm_data.S32flags == DM_SUB_LCD_CONTEXT)
    {
        if (g_dm_cntx.redraw_screen_sub == REDRAW_2_REDRAW)
        {
            return MMI_TRUE;
        }
        else
        {
            return MMI_FALSE;
        }
    }
    else
#endif /* __MMI_SUBLCD__ */ 
    {
        if (g_dm_cntx.redraw_screen_main == REDRAW_2_REDRAW)
        {
            return MMI_TRUE;
        }
        else
        {
            return MMI_FALSE;
        }
    }
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  dm_check_image_bound
 * DESCRIPTION
 *  check image bounder controlled by draw manager
 * PARAMETERS
 *  index       [IN]        Index of images
 *  point       [IN]        Pen point
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL dm_check_image_bound(U16 index, mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (PEN_CHECK_BOUND(
                point.x,
                point.y,
                dm_image_array[index].x,
                dm_image_array[index].y,
                dm_image_array[index].width,
                dm_image_array[index].height));
}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  dm_get_layer_handle
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_HANDLE dm_get_layer_handle(U8 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return dm_layers[index];
}


/*****************************************************************************
 * FUNCTION
 *  dm_get_layer_handle_ptr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_HANDLE *dm_get_layer_handle_ptr(U8 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return &(dm_layers[index]);
}

/*****************************************************************************
 * FUNCTION
 *  dm_reset_layer_handle
 * DESCRIPTION
 *  it's a patch only used for lcd_init now
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
void dm_reset_layer_handle(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 idx;
    gdi_handle handle_ptr;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
            for (idx = 0; idx < dm_layers_count; idx++)
            {
                dm_layers[idx] = GDI_ERROR_HANDLE;
            }
            dm_layers_count = 0;
            gdi_layer_get_active(&handle_ptr);
            gdi_layer_set_blt_layer(handle_ptr ,0,0,0);
}



/*****************************************************************************
 * FUNCTION
 *  dm_add_percentage_bar
 * DESCRIPTION
 *  
 * PARAMETERS
 *  init_value              [IN]   
 *  string                  [IN]
 *  bar_color               [IN]        
 *  bg_image                [IN]        
 *  left_side_image         [IN]        
 *  right_side_image        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U16 dm_add_percentage_bar(U16 init_value, U8 *string, color_t bar_color, U16 bg_image, U16 left_side_image, U16 right_side_image)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_percentage_bar_struct *act_bar = &dm_percentage_bar_array[dm_percentage_bar_count];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    act_bar->value = init_value;
    act_bar->bar_color = bar_color;
    act_bar->bg_image = bg_image;
    act_bar->left_side_image = left_side_image;
    act_bar->right_side_image = right_side_image;
    act_bar->progres_string = string;

    return dm_percentage_bar_count++;
}


/*****************************************************************************
 * FUNCTION
 *  dm_update_percentage_bar_value
 * DESCRIPTION
 *  
 * PARAMETERS
 *  id          [IN]        
 *  value       [IN]        
 *  string      [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void dm_update_percentage_bar_value(S32 id, U16 value, U8* string)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_percentage_bar_array[id].value = value;
    dm_percentage_bar_array[id].progres_string = string;
}


/*****************************************************************************
 * FUNCTION
 *  dm_change_percentage_bar_color
 * DESCRIPTION
 *  
 * PARAMETERS
 *  id      [IN]        
 *  c       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void dm_change_percentage_bar_color(S32 id, color_t c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dm_percentage_bar_array[id].bar_color = c;
}


/*****************************************************************************
 * FUNCTION
 *  dm_redraw_percentage_bar
 * DESCRIPTION
 *  
 * PARAMETERS
 *  id      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void dm_redraw_percentage_bar(S32 id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_percentage_bar_struct *act_bar = &dm_percentage_bar_array[id];

    S32 x = act_bar->x;
    S32 y = act_bar->y;
    S32 width = act_bar->width;
    S32 height = act_bar->height;
    U16 str_buf[5];
    S32 str_width = 0, str_height = 0;
    S32 img_x = 0, img_y = 0;
    S32 img_width = 0, img_height = 0;
    S32 left_image_width = 0, left_image_height = 0;
    S32 right_image_width = 0, right_image_height = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (act_bar->progres_string == NULL)
    {
        gui_itoa(act_bar->value, (UI_string_type) & str_buf, 10);
        UCS2StrAppendChar((S8*)&str_buf,'%');
        gui_measure_string((UI_string_type) str_buf, &str_width, &str_height);
    }
    else
    {
        gui_measure_string((UI_string_type) act_bar->progres_string, &str_width, &str_height);
    }
    gdi_image_get_dimension_id(act_bar->bg_image, &img_width, &img_height);
    gdi_image_get_dimension_id(act_bar->left_side_image, &left_image_width, &left_image_height);
    gdi_image_get_dimension_id(act_bar->right_side_image, &right_image_width, &right_image_height);

    /* clear background */
    gdi_draw_solid_rect(x, y, x + width - 1, y + height - 1, GDI_COLOR_TRANSPARENT);

    /* update text */
    gui_move_text_cursor(x + ((width - str_width) >> 1), y + ((height - str_height - img_height) / 3));
    gui_set_text_color(gui_color(0,0,0));
    
    if (act_bar->progres_string == NULL)
    {
        gui_print_text((UI_string_type) str_buf);
    }
    else
    {
        gui_print_text((UI_string_type) act_bar->progres_string);
    }

    /* update image */
    img_x = x + ((width - img_width - left_image_width - right_image_width) >> 1);
    img_y = y + height - ((height - str_height - img_height) / 3 + img_height);
    gdi_draw_solid_rect(
        img_x + left_image_width,
        img_y + 1,
        img_x + left_image_width + ((img_width * act_bar->value) / 100),
        img_y + img_height - 2,
        gdi_act_color_from_rgb(255, act_bar->bar_color.r, act_bar->bar_color.g, act_bar->bar_color.b));
    gdi_image_draw(img_x, img_y, (U8*) get_image(act_bar->left_side_image));
    gdi_image_draw(img_x + left_image_width - 1, img_y, (U8*) get_image(act_bar->bg_image));
    gdi_image_draw(img_x + left_image_width + img_width - 1, img_y, (U8*) get_image(act_bar->right_side_image));

    gdi_layer_blt_previous(x, y, x + width - 1, y + height - 1);
}
S32 dm_get_current_catID(void)
{
    return g_dm_data.S32CatId;
}
#endif /* __MMI_DRAW_MANAGER__ */ 

