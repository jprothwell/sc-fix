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
 *   wgui_categories_popup.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Popup screen related categories.
 * Author:
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"
#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#undef __NEWSIMULATOR
#include "globaldefs.h"
#include "gui_data_types.h"
#include "custdataprots.h"
#include "custmenures.h"
#include "eventsdef.h"
#include "gui_themes.h"
#include "wgui.h"
#include "custthemesres.h"
#include "gdi_include.h"
#include "wgui_inputs.h"
#include "wgui_categories.h"
#include "wgui_softkeys.h"
#include "wgui_status_icons.h"
#include "wgui_categories_popup.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_defs.h"
#include "timerevents.h"
#include "historygprot.h"       /* __mmi_ui_small_screen_support__ */
#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)
#include "wgui_touch_screen.h"
#include "phonesetupgprots.h"
#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 

#include "wgui_draw_manager.h"
#include "commonscreens.h"      /* 101705 */
#include "quickaccessmenuitemprot.h"
#include "screenrotationgprot.h"
#include "debuginitdef.h"
#include "idleappdef.h"
#include "idleappprot.h"

extern UI_string_type MMI_message_string;
extern PU8 MMI_message_icon;

/*
 * extern S16 Painty1;
 * extern S16 Painty2;
 */
extern S32 wgui_image_clip_y1;
extern S32 wgui_image_clip_y2;
extern S16 status_icon;
extern FuncPtr PopupFunc;

//micha1021, Joyce Requirement, Here to change pop up window background and color
//Check in Main Trunk ONLY
//#ifdef __MMI_PLUTO_GPRS__
UI_filled_area greywgui_pop_up_dialog_background =
    {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_DOUBLE_BORDER | UI_FILLED_AREA_ROUNDED_BORDER |
UI_FILLED_AREA_SHADOW_DOUBLE_LINE | UI_FILLED_AREA_SHADOW,
    UI_NULL_IMAGE,
    NULL,
    {234, 234, 234, 100},
    {0, 0, 0, 0},
    {142, 204, 145, 100},
    {50, 50, 50, 100},
    0
};

// #endif

#ifdef __MMI_DEFAULT_THEME_1__
UI_filled_area wgui_pop_up_dialog_background =
    {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_DOUBLE_BORDER | UI_FILLED_AREA_ROUNDED_BORDER |
UI_FILLED_AREA_SHADOW_DOUBLE_LINE | UI_FILLED_AREA_SHADOW,
    UI_NULL_IMAGE,
    NULL,
    {246, 255, 171, 100},
    {0, 0, 0, 0},
    {142, 204, 145, 100},
    {50, 50, 50, 100},
    0
};

UI_filled_area wgui_pop_up_dialog_inputbox_background = {UI_FILLED_AREA_TYPE_COLOR,
    UI_NULL_IMAGE,
    NULL,
    {246, 255, 171, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    0
};

#else /* __MMI_DEFAULT_THEME_1__ */ 

UI_filled_area wgui_pop_up_dialog_background =
    {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_DOUBLE_BORDER | UI_FILLED_AREA_ROUNDED_BORDER |
UI_FILLED_AREA_SHADOW_DOUBLE_LINE | UI_FILLED_AREA_SHADOW,
    UI_NULL_IMAGE,
    NULL,
    {255, 255, 255, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {50, 50, 50, 100},
    0
};

UI_filled_area wgui_pop_up_dialog_inputbox_background = {UI_FILLED_AREA_TYPE_COLOR,
    UI_NULL_IMAGE,
    NULL,
    {255, 255, 255, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    0
};

#endif /* __MMI_DEFAULT_THEME_1__ */ 

UI_filled_area borderwgui_pop_up_dialog_background = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_SINGLE_BORDER,
    UI_NULL_IMAGE,
    NULL,
    {255, 255, 255, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {50, 50, 50, 100},
    0
};

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
extern BOOL IsPopupSavedCharColor();
extern void PopupSaveCharColor();
extern void PopupChangeCharColor();
extern void PopupResetCharColor();
#endif

/*****************************************************************************
 * FUNCTION
 *  ShowCommonCategoryPopupScreen
 * DESCRIPTION
 *  Displays the category63 screen
 * PARAMETERS
 *  message             [IN]        Message string
 *  history_buffer      [IN]        History buffer
 *  UI_image_ID_type (?)      [IN]        Icon        Message icon
 * RETURNS
 *  void
 *****************************************************************************/
	#include "quickaccessmenuitem_def.h"
#ifdef TGT_GALLITE_G800
extern color_t black_color;
#endif
void ShowCommonCategoryPopupScreen(UI_string_type message, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 l;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);
    gui_lock_double_buffer();
    entry_full_screen();
    clear_category_screen_key_handlers();
    clear_left_softkey();
    clear_right_softkey();
    MMI_message_string = message;
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
#if defined(__PROJECT_GALLITE_C01__)
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW
        | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND
        | UI_MULTI_LINE_INPUT_BOX_AUTO_DISABLE_SCROLLBAR
        | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE;
#else
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW
        | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE;
#endif
    MMI_multiline_inputbox.ext_flags |= GUI_MULTI_LINE_INPUT_BOX_TRUNCATE_CONTENT;
#ifdef TGT_GALLITE_G800
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
    MMI_multiline_inputbox.normal_text_color = black_color;
#endif 
#endif
	/*Wangzl Add Start For 7312 Ver: Multi_Language  on 2008-1-29 16:51 */
	#if defined(__MMI_LANG_FRENCH__)&& (defined(__MMI_MAINLCD_128X160__) || defined(__MMI_MAINLCD_160X128__)||defined (__MMI_MAINLCD_128X128__))
	if(KPAD_SCR_UNLOCK_FIRST_HELP_MESSAGE==GetActiveScreenId())
		MMI_multiline_inputbox.text_font = &MMI_small_font;
	#endif
	/*Wangzl Add End  For 7312 Ver: Multi_Language  on 2008-1-29 16:51 */

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	PopupChangeCharColor();
#endif

    gui_unlock_double_buffer();
}


/*****************************************************************************
 * FUNCTION
 *  ShowCommonCategoryPopupScreenType2
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCommonCategoryPopupScreenType2(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        UI_string_type message,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 l;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);
    gui_lock_double_buffer();
     
    entry_full_screen();
     
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    if (title)
    {
        MMI_title_string = (UI_string_type) get_string(title);
    }
    else
    {
        MMI_title_string = NULL;
    }
    if (MMI_title_icon)
    {
        MMI_title_icon = (PU8) get_image(title_icon);
    }
    else
    {
        MMI_title_icon = NULL;
    }
    MMI_message_string = message;
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW
        | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE;
    MMI_multiline_inputbox.ext_flags |= GUI_MULTI_LINE_INPUT_BOX_TRUNCATE_CONTENT;
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	PopupChangeCharColor();
#endif
    gui_unlock_double_buffer();
}

/*****************************************************************************
 * FUNCTION
 *  ShowCommonCategoryPopupScreenType2
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title                   [IN]        
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCommonCategoryPopupScreenType3(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        UI_string_type message,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 l;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(history_buffer);
    gui_lock_double_buffer();
     
    entry_full_screen();
     
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    if (title)
    {
        MMI_title_string = (UI_string_type) get_string(title);
    }
    else
    {
        MMI_title_string = NULL;
    }
    if (MMI_title_icon)
    {
        MMI_title_icon = (PU8) get_image(title_icon);
    }
    else
    {
        MMI_title_icon = NULL;
    }
    MMI_message_string = message;
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW
        | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND
        | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE;
    MMI_multiline_inputbox.ext_flags |= GUI_MULTI_LINE_INPUT_BOX_TRUNCATE_CONTENT;
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	PopupChangeCharColor();
#endif
    gui_unlock_double_buffer();
}

/*****************************************************************************
 * FUNCTION
 *  wgui_draw_pop_up_screen_background
 * DESCRIPTION
 *  draw the background of popup screns
 * PARAMETERS
 *  x1                      [IN]        Start x position
 *  y1                      [IN]        Start y position
 *  x2                      [IN]        End x position
 *  y2                      [IN]        End y posityion
 *  f                       [IN]        Filled pattern
 *  enable_3D_effect        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_draw_pop_up_screen_background(S32 x1, S32 y1, S32 x2, S32 y2, UI_filled_area *f, MMI_BOOL enable_3D_effect)
{
#ifdef __MMI_MAINLCD_240X320__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
#if 0
#endif /* 0 */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (enable_3D_effect == MMI_TRUE)
    {
        /* Doing nothing */
    }
    //gui_draw_gradient_rounded_rectangle(x1, y1, x2, y2, current_MMI_theme->scrollbar_indicator_filler->c, f->c, 6);
    gui_draw_gradient_rounded_rectangle(x1, y1, x2, y2, f->border_color, f->c, 6);//091506 popup border
    gui_fill_rectangle(x1 + 6, y1 + 6, x2 - 6, y2 - 6, f->c);
     

#else /* __MMI_MAINLCD_240X320__ */ 
#if defined(__MMI_MAINLCD_220X176__)
	gdi_image_draw_id(0,UI_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-70,IMG_POPUP_BG);//70 is IMG_POPUP_BG height.
	gdi_draw_solid_rect(0,y2,UI_device_width,UI_device_height,GDI_COLOR_DARK_GRAY);
/*
	gui_draw_rectangle(x1, y1, x2, y2, gui_color(255,255,255));
	gui_fill_rectangle(x1 + 1, y1 + 1, x2 - 1, y2 -1 ,gui_color(0,64,128));
	gui_putpixel(x1+1,y1+1,gui_color(255,255,255));
	gui_putpixel(x1+1,y2-1,gui_color(255,255,255));
	gui_putpixel(x2-1,y1+1,gui_color(255,255,255));
	gui_putpixel(x2-1,y2-1,gui_color(255,255,255));
	gdi_draw_solid_rect(x1,y2+1,x2,UI_device_height,GDI_COLOR_DARK_GRAY);*/
#else
    gui_fill_rectangle(x1 + 1, y1 + 7, x2 - 1, y2 - 7, f->c);
    gui_draw_vertical_line(y1 + 6, y2 - 6, x1, f->border_color);
    gui_draw_vertical_line(y1 + 6, y2 - 6, x2, f->border_color);
    gui_draw_horizontal_line(x1 + 6, x2 - 6, y1, f->border_color);
    gui_draw_horizontal_line(x1 + 6, x2 - 6, y2, f->border_color);
    gui_draw_horizontal_line(x1 + 6, x2 - 6, y1 + 1, f->c);
    gui_draw_horizontal_line(x1 + 4, x2 - 4, y1 + 2, f->c);
    gui_draw_horizontal_line(x1 + 3, x2 - 3, y1 + 3, f->c);
    gui_draw_horizontal_line(x1 + 2, x2 - 2, y1 + 4, f->c);
    gui_draw_horizontal_line(x1 + 2, x2 - 2, y1 + 5, f->c);
    gui_draw_horizontal_line(x1 + 1, x2 - 1, y1 + 6, f->c);
    gui_draw_horizontal_line(x1 + 6, x2 - 6, y2 - 1, f->c);
    gui_draw_horizontal_line(x1 + 4, x2 - 4, y2 - 2, f->c);
    gui_draw_horizontal_line(x1 + 3, x2 - 3, y2 - 3, f->c);
    gui_draw_horizontal_line(x1 + 2, x2 - 2, y2 - 4, f->c);
    gui_draw_horizontal_line(x1 + 2, x2 - 2, y2 - 5, f->c);
    gui_draw_horizontal_line(x1 + 1, x2 - 1, y2 - 6, f->c);
    gui_putpixel(x1 + 5, y1 + 1, f->border_color);
    gui_putpixel(x1 + 4, y1 + 1, f->border_color);
    gui_putpixel(x1 + 3, y1 + 2, f->border_color);
    gui_putpixel(x1 + 2, y1 + 3, f->border_color);
    gui_putpixel(x1 + 1, y1 + 4, f->border_color);
    gui_putpixel(x1 + 1, y1 + 5, f->border_color);
    gui_putpixel(x2 - 5, y1 + 1, f->border_color);
    gui_putpixel(x2 - 4, y1 + 1, f->border_color);
    gui_putpixel(x2 - 3, y1 + 2, f->border_color);
    gui_putpixel(x2 - 2, y1 + 3, f->border_color);
    gui_putpixel(x2 - 1, y1 + 4, f->border_color);
    gui_putpixel(x2 - 1, y1 + 5, f->border_color);

    gui_putpixel(x1 + 5, y2 - 1, f->border_color);
    gui_putpixel(x1 + 4, y2 - 1, f->border_color);
    gui_putpixel(x1 + 3, y2 - 2, f->border_color);
    gui_putpixel(x1 + 2, y2 - 3, f->border_color);
    gui_putpixel(x1 + 1, y2 - 4, f->border_color);
    gui_putpixel(x1 + 1, y2 - 5, f->border_color);
    gui_putpixel(x2 - 5, y2 - 1, f->border_color);
    gui_putpixel(x2 - 4, y2 - 1, f->border_color);
    gui_putpixel(x2 - 3, y2 - 2, f->border_color);
    gui_putpixel(x2 - 2, y2 - 3, f->border_color);
    gui_putpixel(x2 - 1, y2 - 4, f->border_color);
    gui_putpixel(x2 - 1, y2 - 5, f->border_color);
    if (enable_3D_effect == MMI_TRUE)
    {
        gui_draw_horizontal_line(x1 + 7, x2 - 6, y2 + 1, f->border_color);
        gui_draw_vertical_line(y1 + 7, y2 - 6, x2 + 1, f->border_color);

        gui_putpixel(x2 - 5, y2, f->border_color);
        gui_putpixel(x2 - 5, y2 + 1, f->border_color);
        gui_putpixel(x2 - 4, y2, f->border_color);
        gui_putpixel(x2 - 4, y2 + 1, f->border_color);
        gui_putpixel(x2 - 3, y2, f->border_color);
        gui_putpixel(x2 - 3, y2 - 1, f->border_color);
        gui_putpixel(x2 - 2, y2 - 2, f->border_color);
        gui_putpixel(x2 - 2, y2 - 1, f->border_color);
        gui_putpixel(x2 - 1, y2 - 3, f->border_color);
        gui_putpixel(x2 - 1, y2 - 2, f->border_color);
        gui_putpixel(x2 - 0, y2 - 4, f->border_color);
        gui_putpixel(x2 - 0, y2 - 3, f->border_color);
        gui_putpixel(x2, y2 - 5, f->border_color);
        gui_putpixel(x2 + 1, y2 - 5, f->border_color);
        gui_putpixel(x2 + 1, y2 - 4, f->border_color);
    }
#endif	
#endif /* __MMI_MAINLCD_240X320__ */ 
}


/*****************************************************************************
 * FUNCTION
 *  wgui_draw_pop_up_screen_background_style2
 * DESCRIPTION
 *  draw the background of popup screns
 * PARAMETERS
 *  x1          [IN]        Start x position
 *  y1          [IN]        Start y position
 *  x2          [IN]        End x position
 *  y2          [IN]        End y posityion
 *  f           [IN]        Filled pattern
 *  top         [IN]        Whether to display top border
 *  bottom      [IN]        Whether to display bottom border
 *  left        [IN]        Whether to display left border
 *  right       [IN]        Whether to display right border
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_draw_pop_up_screen_background_style2(
        S32 x1,
        S32 y1,
        S32 x2,
        S32 y2,
        UI_filled_area *f,
        MMI_BOOL top,
        MMI_BOOL bottom,
        MMI_BOOL left,
        MMI_BOOL right)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 fy1, fy2, fx1, fx2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (top)
    {
        fy1 = y1 + 6;
        gui_draw_horizontal_line(x1 + 6, x2 - 6, y1 + 1, f->c);
        gui_draw_horizontal_line(x1 + 4, x2 - 4, y1 + 2, f->c);
        gui_draw_horizontal_line(x1 + 3, x2 - 3, y1 + 3, f->c);
        gui_draw_horizontal_line(x1 + 2, x2 - 2, y1 + 4, f->c);
        gui_draw_horizontal_line(x1 + 2, x2 - 2, y1 + 5, f->c);

        gui_draw_horizontal_line(x1 + 6, x2 - 6, y1, f->border_color);
        gui_putpixel(x1 + 5, y1 + 1, f->border_color);
        gui_putpixel(x1 + 4, y1 + 1, f->border_color);
        gui_putpixel(x1 + 3, y1 + 2, f->border_color);
        gui_putpixel(x1 + 2, y1 + 3, f->border_color);
        gui_putpixel(x1 + 1, y1 + 4, f->border_color);
        gui_putpixel(x1 + 1, y1 + 5, f->border_color);
        gui_putpixel(x2 - 5, y1 + 1, f->border_color);
        gui_putpixel(x2 - 4, y1 + 1, f->border_color);
        gui_putpixel(x2 - 3, y1 + 2, f->border_color);
        gui_putpixel(x2 - 2, y1 + 3, f->border_color);
        gui_putpixel(x2 - 1, y1 + 4, f->border_color);
        gui_putpixel(x2 - 1, y1 + 5, f->border_color);
    }
    else
    {
        fy1 = y1;
    }

    if (bottom)
    {
        fy2 = y2 - 6;

        gui_draw_horizontal_line(x1 + 6, x2 - 6, y2 - 1, f->c);
        gui_draw_horizontal_line(x1 + 4, x2 - 4, y2 - 2, f->c);
        gui_draw_horizontal_line(x1 + 3, x2 - 3, y2 - 3, f->c);
        gui_draw_horizontal_line(x1 + 2, x2 - 2, y2 - 4, f->c);
        gui_draw_horizontal_line(x1 + 2, x2 - 2, y2 - 5, f->c);

        gui_draw_horizontal_line(x1 + 6, x2 - 6, y2, f->border_color);
        gui_putpixel(x1 + 5, y2 - 1, f->border_color);
        gui_putpixel(x1 + 4, y2 - 1, f->border_color);
        gui_putpixel(x1 + 3, y2 - 2, f->border_color);
        gui_putpixel(x1 + 2, y2 - 3, f->border_color);
        gui_putpixel(x1 + 1, y2 - 4, f->border_color);
        gui_putpixel(x1 + 1, y2 - 5, f->border_color);
        gui_putpixel(x2 - 5, y2 - 1, f->border_color);
        gui_putpixel(x2 - 4, y2 - 1, f->border_color);
        gui_putpixel(x2 - 3, y2 - 2, f->border_color);
        gui_putpixel(x2 - 2, y2 - 3, f->border_color);
        gui_putpixel(x2 - 1, y2 - 4, f->border_color);
        gui_putpixel(x2 - 1, y2 - 5, f->border_color);
    }
    else
    {
        fy2 = y2;
    }

    if (left)
    {
        fx1 = x1 + 1;
        gui_draw_vertical_line(fy1, fy2, x1, f->border_color);
    }
    else
    {
        fx1 = x1;
    }

    if (right)
    {
        fx2 = x2 - 1;
        gui_draw_vertical_line(fy1, fy2, x2, f->border_color);
    }
    else
    {
        fx2 = x2;
    }

    gui_fill_rectangle(fx1, fy1, fx2, fy2, f->c);
}

extern void wgui_set_animation_image_y(S32 x, S32 y, PU8 img);


/*****************************************************************************
 * FUNCTION
 *  ShowCategory62Screen
 * DESCRIPTION
 *  Displays the category62 screen
 * PARAMETERS
 *  message             [IN]        Message string
 *  message_icon        [IN]        
 *  history_buffer      [IN]        History buffer
 *  UI_image_ID_type (?)      [IN]        Icon        Message icon
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory62Screen(U16 message, U16 message_icon, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
#endif 
     

    ShowCommonCategoryPopupScreen(get_string(message), history_buffer);
    ExitCategoryFunction = ExitCategory62Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY62_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    if (message_icon)
    {
        dm_add_image(get_image(message_icon), NULL, NULL);
    }
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory62Screen
 * DESCRIPTION
 *  Exits the category62 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory62Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    reset_small_screen();
#endif 
     
    gui_hide_animations();
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory63Screen
 * DESCRIPTION
 *  Displays the category63 screen
 * PARAMETERS
 *  message             [IN]        Message string
 *  message_icon        [IN]        
 *  history_buffer      [IN]        History buffer
 *  UI_image_ID_type (?)      [IN]        Icon        Message icon
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory63Screen(U8 *message, U16 message_icon, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
#endif 
     

    ShowCommonCategoryPopupScreen((UI_string_type) message, history_buffer);
    ExitCategoryFunction = ExitCategory62Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY62_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_add_image(get_image(message_icon), NULL, NULL);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory64Screen
 * DESCRIPTION
 *  Displays the category64 screen
 * PARAMETERS
 *  message             [IN]        Message string
 *  message_icon        [IN]        
 *  history_buffer      [IN]        History buffer
 *  UI_image_ID_type (?)      [IN]        Icon        Message icon
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory64Screen(U16 message, U16 message_icon, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
#endif 
     
#ifdef TGT_GALLITE_G800
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    wgui_set_wallpaper_on_bottom(MMI_TRUE);
#endif
#endif

    ShowCommonCategoryPopupScreen(get_string(message), history_buffer);
    ExitCategoryFunction = ExitCategory62Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY64_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_add_image(get_image(message_icon), NULL, NULL);
    dm_redraw_category_screen();
}

 
#if (defined __MMI_TOUCH_SCREEN__)


/*****************************************************************************
 * FUNCTION
 *  Cat65ImagePenDownHdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Cat65ImagePenDownHdlr(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (PopupFunc == NULL)
    {
        PopupTimerFlagEnable();
        PopupCloseByPressAnyKey();
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }

}
#endif /* (defined __MMI_TOUCH_SCREEN__) */ 
/* Len end 20050722 */


/*****************************************************************************
 * FUNCTION
 *  ShowCategory65Screen
 * DESCRIPTION
 *  Displays the category65 screen
 * PARAMETERS
 *  message             [IN]        Message string
 *  message_icon        [IN]        
 *  history_buffer      [IN]        History buffer
 *  UI_image_ID_type (?)      [IN]        Icon        Message icon
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory65Screen(U8 *message, U16 message_icon, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
#endif 
     

    ShowCommonCategoryPopupScreen((UI_string_type) message, history_buffer);
#ifdef __MMI_TOUCH_SCREEN__
    wgui_register_category_screen_control_area_pen_handlers(Cat65ImagePenDownHdlr, MMI_PEN_EVENT_DOWN);
#endif 
    ExitCategoryFunction = ExitCategory62Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY64_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_add_image(get_image(message_icon), NULL, NULL);
    dm_redraw_category_screen();
}

#if defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__)

typedef struct
{
    S16 image_x;    /* left-top corner of image */
    S16 image_y;
    U16 image_id;
    U16 state;
} gui_calibration_screen_struct;

static gui_calibration_screen_struct g_gui_calibration_screen_cntx;


/*****************************************************************************
 * FUNCTION
 *  UpdateCalibrationScreen
 * DESCRIPTION
 *  Updates Multiline buffer contents before redrawing calibration setting screens.
 * PARAMETERS
 *  image_id        [IN]        
 *  cali_x          [IN]        The position of central point of image (i.e. the calibration point)
 *  cali_y          [IN]        
 *  string          [IN]        
 *  state           [IN]        Casted from PHNSET_CALIBRATION_STATE_ENUM
 *  redraw          [IN]        Redraw the category screen
 *  image(?)        [IN]        
 * RETURNS
 *  void
 
 *  We should avoid overlapping of image and text
 *****************************************************************************/
	#include "settingdefs.h"
void UpdateCalibrationScreen(U16 image_id, S16 cali_x, S16 cali_y, U16 string, U16 state, BOOL redraw)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 l, fh, image_width = 0, image_height = 0;
    S32 yloc = 0;    
    S32 maxht;      /* maximum height of Multiline Box */
    PU8 image;
    S32 image_x1, image_y1, image_x2, image_y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_gui_calibration_screen_cntx.image_id = image_id;
    //g_gui_calibration_screen_cntx.state = state;

    if (image_id)
    {
        image = get_image((UI_image_ID_type ) image_id);
        gui_measure_image(image, &image_width, &image_height);

        image_x1 = g_gui_calibration_screen_cntx.image_x = (S16) (cali_x - (image_width >> 1));
        image_y1 = g_gui_calibration_screen_cntx.image_y = (S16) (cali_y - (image_height >> 1));
        image_x2 = image_x1 + image_width - 1;
        image_y2 = image_x1 + image_height - 1;

        MMI_DBG_ASSERT(image_x1 >= 0 && image_x2 < UI_device_width && image_y1 >= 0 && image_y2 < UI_device_height);
    }
    else
    {
        image = NULL;
        image_x1 = image_y1 = image_x2 = image_y2 = 0;
    }

    /* Compute inputbox height */
    MMI_message_string = (UI_string_type) GetString(string);
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW
        | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE;
	/*Wangzl Add Start For 7316 Ver: Multi_Language  on 2008-1-29 16:52 */
	#if defined(__MMI_LANG_FRENCH__)&&(defined(__MMI_MAINLCD_128X160__)||defined(__MMI_MAINLCD_160X128__)||defined (__MMI_MAINLCD_128X128__))
	if(STR_CALIBRATION_WELCOME_TEXT == string)
		MMI_multiline_inputbox.text_font = &MMI_small_font;
	#endif
	/*Wangzl Add End  For 7316 Ver: Multi_Language  on 2008-1-29 16:52 */
    resize_multiline_inputbox(UI_device_width - 2, UI_device_height);
    show_multiline_inputbox_no_draw();
    fh = get_multiline_inputbox_line_height();

    /* Move the inputbox to appropriate position */
#ifdef __MMI_MAINLCD_128X128__
    maxht = (MMI_multiline_inputbox.n_lines * fh) + MULTILINE_INPUTBOX_HEIGHT_PAD + MMI_multiline_inputbox.text_y + 10;
#else 
    maxht = (MMI_multiline_inputbox.n_lines * fh) + MULTILINE_INPUTBOX_HEIGHT_PAD + MMI_multiline_inputbox.text_y;
#endif 

    if (maxht > UI_device_height)
    {
        maxht = UI_device_height;
    }
    if( cali_y < UI_device_height/3 ||
            cali_y > UI_device_height/3)
    { 
        yloc = UI_device_height/2;
    }
    else if( cali_y >UI_device_height/2)
    { 
        yloc = UI_device_height/4;
    }
    else
    { 
        yloc = UI_device_height/4*3;
    }
    resize_multiline_inputbox(UI_device_width - 2, maxht);
    move_multiline_inputbox(1, yloc);
    if (redraw)
    {
        RedrawCalibrationScreen();
    }
}


/*****************************************************************************
 * FUNCTION
 *  ExitCalibrationScreen
 * DESCRIPTION
 *  Exits Calibration Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCalibrationScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_hide_animations();
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}


/*****************************************************************************
 * FUNCTION
 *  RedrawCalibrationScreen
 * DESCRIPTION
 *  Redraws Calibration Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCalibrationScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PU8 image = NULL;
    gdi_handle handle;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_hide_animations();

    if (g_gui_calibration_screen_cntx.image_id)
    {
        image = get_image((UI_image_ID_type ) g_gui_calibration_screen_cntx.image_id);
    }

    gui_push_clip();
    gui_reset_clip();

    clear_screen();
    show_multiline_inputbox();

    gui_reset_clip();
    if (image)
    {
        gdi_image_draw_animation(
            g_gui_calibration_screen_cntx.image_x,
            g_gui_calibration_screen_cntx.image_y,
            image,
            (gdi_handle*) & handle);
    }
    gui_pop_clip();

    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCalibrationScreen
 * DESCRIPTION
 *  Shows Calibration Screen
 * PARAMETERS
 *  image_id        [IN]        
 *  cali_x          [IN]        
 *  cali_y          [IN]        
 *  string          [IN]        
 *  state           [IN]        
 *  image(?)        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCalibrationScreen(U16 image_id, S16 cali_x, S16 cali_y, U16 string, U16 state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    entry_full_screen();
    MMI_menu_shortcut_number = -1;

    gui_lock_double_buffer();
    clear_category_screen_key_handlers();
    clear_left_softkey();
    clear_right_softkey();
    UpdateCalibrationScreen(image_id, cali_x, cali_y, string, state, MMI_FALSE);
    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCalibrationScreen;
    RedrawCategoryFunction = RedrawCalibrationScreen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    RedrawCalibrationScreen();
}

#ifdef __MMI_HANDWRITING_PAD__

static U8 g_gui_control_area[TP_AREA_MAX_NUM];


/*****************************************************************************
 * FUNCTION
 *  UpdateTestControlAreaScreen
 * DESCRIPTION
 *  Updates Multiline buffer contents before redrawing test control area screens.
 * PARAMETERS
 *  pos         [IN]        The position of the click point
 *  string      [IN]        
 *  state       [IN]        Casted from PHNSET_CALIBRATION_STATE_ENUM
 *  redraw      [IN]        Redraw the category screen
 * RETURNS
 *  the state in testing control area screen
 
 *  We should avoid overlapping of image and text
 *****************************************************************************/
U16 UpdateTestControlAreaScreen(mmi_pen_point_struct pos, U16 string, U16 state, BOOL redraw)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 l, fh;
    S32 yloc;   /* start y coordinate */
    S32 maxht;  /* maximum height of Multiline Box */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Compute inputbox height */
    MMI_message_string = (UI_string_type) GetString(string);
    l = gui_strlen(MMI_message_string);
    create_multiline_inputbox_set_buffer(MMI_message_string, l, l, 0);
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW
        | UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND
        | UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE;
    resize_multiline_inputbox(UI_device_width - 2, UI_device_height);
    show_multiline_inputbox_no_draw();
    fh = get_multiline_inputbox_line_height();

    /* Move the inputbox to appropriate position */
#ifdef __MMI_UI_STYLE_1__
    maxht = (MMI_multiline_inputbox.n_lines * fh) + MULTILINE_INPUTBOX_HEIGHT_PAD + MMI_multiline_inputbox.text_y + 10;
#else 
    maxht = (MMI_multiline_inputbox.n_lines * fh) + MULTILINE_INPUTBOX_HEIGHT_PAD + MMI_multiline_inputbox.text_y;
#endif 

    if (maxht > UI_device_height)
    {
        maxht = UI_device_height;
    }

    switch (state)
    {
        case TEST_CONTROL_AREA_WELCOME_STATE:
            memset(g_gui_control_area, 0, sizeof(U8) * TP_AREA_MAX_NUM);
            yloc = 20;  /* (UI_device_height>>1) - (maxht >> 1); */
            state = TEST_CONTROL_AREA_TESTING_STATE;
            break;

        case TEST_CONTROL_AREA_TESTING_STATE:
        {
            S32 i;

            yloc = 20;  /* (UI_device_height>>1) - (maxht >> 1); */
            if ((i = mmi_pen_check_inside_control_area(pos)) >= 0)
            {
                g_gui_control_area[i] = MMI_TRUE;
            }

            state = TEST_CONTROL_AREA_FINISH_STATE;
            for (i = 0; i < TP_AREA_MAX_NUM; i++)
            {
                if (IS_ENABLE_FLAG(tp_area_table[i].flag, TP_CONTROL_AREA) && (g_gui_control_area[i] != MMI_TRUE))
                {
                    state = TEST_CONTROL_AREA_TESTING_STATE;
                    break;
                }
            }
        }
            break;

        default:
            MMI_ASSERT(0);
            state = TEST_CONTROL_AREA_WELCOME_STATE;
            break;
    }

    resize_multiline_inputbox(UI_device_width - 2, maxht);
    move_multiline_inputbox(1, yloc);

    if (redraw)
    {
        RedrawTestControlAreaScreen();
    }
    return state;
}


/*****************************************************************************
 * FUNCTION
 *  ExitTestControlAreaScreen
 * DESCRIPTION
 *  Exits Control Area Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitTestControlAreaScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}


/*****************************************************************************
 * FUNCTION
 *  RedrawTestControlAreaScreen
 * DESCRIPTION
 *  Redraws Control Area Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawTestControlAreaScreen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* gdi_color color[] = {GDI_COLOR_BLACK, GDI_COLOR_GREEN, GDI_COLOR_GRAY, GDI_COLOR_RED, GDI_COLOR_GREEN }; */

    gui_lock_double_buffer();

    gui_push_clip();
    gui_reset_clip();

    clear_screen();
    show_multiline_inputbox();

    gui_reset_clip();

    for (i = 0; i < TP_AREA_MAX_NUM; i++)
    {
        if (IS_ENABLE_FLAG(tp_area_table[i].flag, TP_CONTROL_AREA) && (g_gui_control_area[i] != MMI_TRUE))
        {
            int index = 0;
            mmi_pen_point_struct pos1, pos2;

            pos1 = tp_area_table[i].pos[index];
            index++;
            while ((tp_area_table[i].pos[index].x != -1) && (tp_area_table[i].pos[index].y != -1))
            {
                pos2 = tp_area_table[i].pos[index];
                gdi_draw_line(pos1.x, pos1.y, pos2.x, pos2.y, GDI_COLOR_BLACK);
                pos1 = pos2;
                index++;
            }
            pos2 = tp_area_table[i].pos[0];
            gdi_draw_line(pos1.x, pos1.y, pos2.x, pos2.y, GDI_COLOR_BLACK);

        }
        else
        {
            /* handwriting area */
        }
    }

    gui_pop_clip();

    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  ShowTestControlAreaScreen
 * DESCRIPTION
 *  Shows Control Area Screen
 * PARAMETERS
 *  pos         [IN]        
 *  string      [IN]        
 *  state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ShowTestControlAreaScreen(mmi_pen_point_struct pos, U16 string, U16 state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    entry_full_screen();
    MMI_menu_shortcut_number = -1;

    gui_lock_double_buffer();
    clear_category_screen_key_handlers();
    clear_left_softkey();
    clear_right_softkey();
    UpdateTestControlAreaScreen(pos, string, state, MMI_FALSE);
    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitTestControlAreaScreen;
    RedrawCategoryFunction = RedrawTestControlAreaScreen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    RedrawTestControlAreaScreen();
}
#endif /* __MMI_HANDWRITING_PAD__ */ 

#endif /* defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) */ 

/** Slide 32 ***/


/*****************************************************************************
 * FUNCTION
 *  ExitCategory121Screen
 * DESCRIPTION
 *  Exits the category121 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory121Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    reset_small_screen();
#endif 
    close_status_icons();
    reset_softkeys();
    reset_multiline_inputbox();
    gui_hide_animations();
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}

extern void wgui_set_animation_y(S32 x, S32 y, UI_image_ID_type  img);


/*****************************************************************************
 * FUNCTION
 *  ShowCategory121Screen
 * DESCRIPTION
 *  Displays the category121 screen
 * PARAMETERS
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the right softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        String to display
 *  message_icon            [IN]        Icon to display
 *  history_buffer          [IN]        History buffer
 *  flag                    [IN]        Value shpuld be 0/1 -
 *  no(?)                   [IN]        Border drawn
 *  border(?)               [IN]        Around image and text
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory121Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        U16 message_icon,
        U8 *history_buffer,
        U8 flag)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
#endif 
    ShowCommonCategoryPopupScreenType2(
        0,
        0,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        (UI_string_type) message,
        history_buffer);
#ifndef __MMI_UI_SMALL_SCREEN_SUPPORT__
    ShowStatusIconsTitle();
#endif 
    ExitCategoryFunction = ExitCategory121Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY121_ID;
#ifndef __MMI_UI_SMALL_SCREEN_SUPPORT__
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND | DM_NO_TITLE;
#else 
    dm_data.S32flags = DM_NO_TITLE | DM_NO_STATUS_BAR;
#endif 
    if (flag == 0)
    {
        dm_data.S32flags |= DM_NO_POPUP_BACKGROUND;
    }
    dm_setup_data(&dm_data);

    dm_add_image(get_image(message_icon), NULL, NULL);
    dm_redraw_category_screen();
}

/** Slide 56,57 ****/


/*****************************************************************************
 * FUNCTION
 *  Cate123CategoryControlAreaPenDownHandler
 * DESCRIPTION
 *  Category 123 control area pen down event handler
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  MMI_BOOL
 *****************************************************************************/
 
#if (defined __MMI_TOUCH_SCREEN__)
MMI_BOOL Cate123CategoryControlAreaPenDownHandler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_idle_restart_keypad_lock_timer();
    return MMI_TRUE;
}
#endif /* (defined __MMI_TOUCH_SCREEN__) */ 


/*****************************************************************************
 * FUNCTION
 *  ShowCategory123Screen
 * DESCRIPTION
 *  Displays the category123 screen with status bar at the top
 * PARAMETERS
 *  title                   [IN]        =0 and title_icon=0, then status bar is displayed instead of the title
 *  title_icon              [IN]        
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message                 [IN]        Notification message
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  UI_image_ID_type (?)          [IN]        Image          Notification image
 *  For(?)                  [IN]        Other values, a title is displayed
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory123Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
#endif 
     

    ShowCommonCategoryPopupScreenType2(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        get_string(message),
        history_buffer);
#if (defined __MMI_TOUCH_SCREEN__)
    wgui_register_category_screen_control_area_pen_handlers(
        Cate123CategoryControlAreaPenDownHandler,
        MMI_PEN_EVENT_DOWN);
#endif /* (defined __MMI_TOUCH_SCREEN__) */ 
    ExitCategoryFunction = ExitCategory121Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY123_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    if ((title == 0) && (title_icon == 0))
    {
        dm_data.S32flags |= DM_NO_TITLE;
        ShowStatusIconsTitle();
    }
    else if (!((title == 0xffff) && (title_icon == 0xffff)))
    {
    #if !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__)      
        dm_data.S32flags |= DM_NO_STATUS_BAR;
    #endif 
    }
    else
    {
    #if !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__)      
        dm_data.S32flags |= DM_NO_STATUS_BAR | DM_NO_TITLE;
    #else /* !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__) */ 
        {
            dm_data.S32flags |= DM_NO_TITLE;
            ShowStatusIconsTitle();
        }
    #endif /* !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__) */ 

    }
    dm_setup_data(&dm_data);

    dm_add_image(get_image(message_icon), NULL, NULL);
    dm_redraw_category_screen();
}

/** Slide 56,57: Dynamic version of 123 ****/


/*****************************************************************************
 * FUNCTION
 *  ShowCategory124Screen
 * DESCRIPTION
 *  Displays the category124 screen with status bar at the top
 * PARAMETERS
 *  title                   [IN]        =0 and title_icon=0, then status bar is displayed instead of the title
 *  title_icon              [IN]        
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message                 [IN]        Notification message
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  UI_image_ID_type (?)          [IN]        Image          Notification image
 *  For(?)                  [IN]        Other values, a title is displayed
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory124Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
#endif 
     

    ShowCommonCategoryPopupScreenType2(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        (UI_string_type) message,
        history_buffer);
    ExitCategoryFunction = ExitCategory121Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY123_ID;
    dm_data.S32flags = 0;
    if ((title == 0) && (title_icon == 0))
    {
        dm_data.S32flags |= DM_NO_TITLE;
        ShowStatusIconsTitle();
    }
    else if (!((title == 0xffff) && (title_icon == 0xffff)))
    {
    #if !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__)      
        dm_data.S32flags |= DM_NO_STATUS_BAR;
    #endif 
    }
    else
    {
    #if !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__)      
        dm_data.S32flags |= DM_NO_STATUS_BAR | DM_NO_TITLE;
    #else /* !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__) */ 
        {
            dm_data.S32flags |= DM_NO_TITLE;
            ShowStatusIconsTitle();
        }
    #endif /* !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__) */ 

    }
    dm_setup_data(&dm_data);
    dm_add_image(get_image(message_icon), NULL, NULL);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory151Screen
 * DESCRIPTION
 *  Exits the category151 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory151Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    //#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    //   reset_small_screen();
    //#endif
     

    MMI_multiline_inputbox.normal_filler = current_UI_theme->multi_line_input_box_theme->normal_filler;
    gui_hide_animations();
    reset_softkeys();
    close_status_icons();
    reset_multiline_inputbox();
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
#if defined(__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)        
    leave_full_screen();
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  category151_keyboard_handler
 * DESCRIPTION
 *  set category151 key board handlres
 * PARAMETERS
 *  vkey_code       [IN]        
 *  key_state       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void category151_keyboard_handler(S32 vkey_code, S32 key_state)
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
            case 38:
                multiline_inputbox_previous_line();
                break;
            case 40:
                multiline_inputbox_next_line();
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
 *  GetCategory151HistorySize
 * DESCRIPTION
 *  Gets the size of the history buffer for category69 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  size in U8s of the history buffer
 *****************************************************************************/
S32 GetCategory151HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (sizeof(multiline_inputbox_category_history));
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory151History
 * DESCRIPTION
 *  Gets the history buffer for category69 screen
 * PARAMETERS
 *  history_buffer      [IN]        Is the buffer into which the history data is stored (pre-allocated)
 * RETURNS
 *  Pointer to the history buffer
 *****************************************************************************/
U8 *GetCategory151History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_multiline_inputbox_category_history(MMI_CATEGORY151_ID, history_buffer, MMI_current_input_type);
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory151Screen
 * DESCRIPTION
 *  Displays the category151 screen with status bar at the top
 * PARAMETERS
 *  title                   [IN]        =0 and title_icon=0, then status bar is displayed instead of the title
 *  title_icon              [IN]        
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message                 [IN]        Notification message
 *  message_icon            [?]         
 *  history_buffer          [IN]        History buffer
 *  UI_image_ID_type (?)          [IN]        Image          Notification image
 *  For(?)                  [IN]        Other values, a title is displayed
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory151Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        U8 *message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S16 input_type;  

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    //set_small_screen();
#endif 
     

    ShowCommonCategoryPopupScreenType2(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        (UI_string_type) message,
        history_buffer);

     
    set_multiline_inputbox_category_history(MMI_CATEGORY151_ID, history_buffer, (S16*) & input_type);
    show_multiline_inputbox_no_draw();
     

    ExitCategoryFunction = ExitCategory151Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = GetCategory151History;
    GetCategoryHistorySize = GetCategory151HistorySize;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY151_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    if ((title == 0) && (title_icon == 0))
    {
        dm_data.S32flags |= DM_NO_TITLE;
        ShowStatusIconsTitle();
    }
    else if (!((title == 0xffff) && (title_icon == 0xffff)))
    {
    #if !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__)      
        dm_data.S32flags |= DM_NO_STATUS_BAR;
    #endif 
    }
    else
    {
    #if !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__)      
        dm_data.S32flags |= DM_NO_STATUS_BAR | DM_NO_TITLE;
    #else /* !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__) */ 
        {
            dm_data.S32flags |= DM_NO_TITLE;
            ShowStatusIconsTitle();
        }
    #endif /* !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__) &&!defined(__MMI_MAINLCD_220X176__)*/ 

    }
    dm_setup_data(&dm_data);
    MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND;
    dm_add_image(message_icon, NULL, NULL);
    MMI_multiline_inputbox.normal_filler = &wgui_pop_up_dialog_inputbox_background;
    dm_redraw_category_screen();

}

/*****************************************************************************
 * FUNCTION
 *  ShowCategory151Screen
 * DESCRIPTION
 *  Displays the category151 screen with status bar at the top
 * PARAMETERS
 *  title                   [IN]        =0 and title_icon=0, then status bar is displayed instead of the title
 *  title_icon              [IN]        
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  message                 [IN]        Notification message
 *  message_icon            [?]         
 *  history_buffer          [IN]        History buffer
 *  UI_image_ID_type (?)          [IN]        Image          Notification image
 *  For(?)                  [IN]        Other values, a title is displayed
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory151Screen_ex(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        U8 *message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S16 input_type;  

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    //set_small_screen();
#endif 
     

    ShowCommonCategoryPopupScreenType3(
        title,
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        (UI_string_type) message,
        history_buffer);

     
    set_multiline_inputbox_category_history(MMI_CATEGORY151_ID, history_buffer, (S16*) & input_type);
    show_multiline_inputbox_no_draw();
     

    ExitCategoryFunction = ExitCategory151Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = GetCategory151History;
    GetCategoryHistorySize = GetCategory151HistorySize;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY151_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    if ((title == 0) && (title_icon == 0))
    {
        dm_data.S32flags |= DM_NO_TITLE;
        ShowStatusIconsTitle();
    }
    else if (!((title == 0xffff) && (title_icon == 0xffff)))
    {
    #if !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__)      
        dm_data.S32flags |= DM_NO_STATUS_BAR;
    #endif 
    }
    else
    {
    #if !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__)      
        dm_data.S32flags |= DM_NO_STATUS_BAR | DM_NO_TITLE;
    #else /* !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__)&&!defined(__MMI_MAINLCD_220X176__) */ 
        {
            dm_data.S32flags |= DM_NO_TITLE;
            ShowStatusIconsTitle();
        }
    #endif /* !defined(__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__) &&!defined(__MMI_MAINLCD_220X176__)*/ 

    }
#if defined(__MMI_INSCREEN_MULTILINE_TEXTBOX__)
    SetKeyHandler(multiline_inscreen_inputbox_previous_line, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(multiline_inscreen_inputbox_next_line, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(multiline_inscreen_inputbox_previous_line, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(multiline_inscreen_inputbox_next_line, KEY_VOL_DOWN, KEY_EVENT_DOWN);    
#else
    SetKeyHandler(multiline_inputbox_previous_line, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(multiline_inputbox_next_line, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(multiline_inputbox_previous_line, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(multiline_inputbox_next_line, KEY_VOL_DOWN, KEY_EVENT_DOWN);    
#endif
    dm_setup_data(&dm_data);
    MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND;
    dm_add_image(message_icon, NULL, NULL);
    MMI_multiline_inputbox.normal_filler = &wgui_pop_up_dialog_inputbox_background;
    dm_redraw_category_screen();

}

/*****************************************************************************
 * FUNCTION
 *  ShowCategory162Screen
 * DESCRIPTION
 *  Displays the category62 screen
 * PARAMETERS
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        Message string
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  UI_image_ID_type (?)          [IN]        Icon        Message icon
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory162Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
#endif 
     

    ShowCommonCategoryPopupScreenType2(
        0,
        0,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        get_string(message),
        history_buffer);
    ExitCategoryFunction = ExitCategory62Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY162_ID;
    dm_data.S32flags = DM_NO_TITLE | DM_NO_STATUS_BAR;
    dm_setup_data(&dm_data);
    dm_add_image(get_image(message_icon), NULL, NULL);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory163Screen
 * DESCRIPTION
 *  Displays the category63 screen
 * PARAMETERS
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        Message string
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  UI_image_ID_type (?)          [IN]        Icon        Message icon
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory163Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
#endif 
     

    ShowCommonCategoryPopupScreenType2(
        0,
        0,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        (UI_string_type) message,
        history_buffer);
    ExitCategoryFunction = ExitCategory62Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY162_ID;
    dm_data.S32flags = DM_NO_TITLE | DM_NO_STATUS_BAR;
    dm_setup_data(&dm_data);
    dm_add_image(get_image(message_icon), NULL, NULL);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory164Screen
 * DESCRIPTION
 *  Displays the category64 screen
 * PARAMETERS
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        Message string
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  UI_image_ID_type (?)          [IN]        Icon        Message icon
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory164Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
#endif 
     

    ShowCommonCategoryPopupScreenType2(
        0,
        0,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        get_string(message),
        history_buffer);
    ExitCategoryFunction = ExitCategory62Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY164_ID;
    dm_data.S32flags = DM_NO_TITLE | DM_NO_STATUS_BAR;
    dm_setup_data(&dm_data);
    dm_add_image(get_image(message_icon), NULL, NULL);
    dm_redraw_category_screen();
}

 
S32 Cat165CountDownValue;
FuncPtr Cat165TimerExpireCallBack = UI_dummy_function;


/*****************************************************************************
 * FUNCTION
 *  SetCat165TimerExpireFunc
 * DESCRIPTION
 *  Set popup countdown callback function
 * PARAMETERS
 *  f       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCat165TimerExpireFunc(FuncPtr f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Cat165TimerExpireCallBack = f;
}


/*****************************************************************************
 * FUNCTION
 *  Cat165CountDownCallBack
 * DESCRIPTION
 *  popup countdown callback function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern UI_font_type UI_font;
extern color_t UI_text_color;
extern BOOL r2lMMIFlag;
void Cat165CountDownCallBack(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 buf[6];
    UI_font_type saved_font = UI_font;
    color_t saved_color = UI_text_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (Cat165CountDownValue == 0)
    {
        gui_cancel_timer(Cat165CountDownCallBack);
        if (Cat165TimerExpireCallBack != NULL)
        {
            Cat165TimerExpireCallBack();
        }
        // Not necessary to reset this callback because it is already reset by exit function
        // Cat165TimerExpireCallBack = UI_dummy_function;
    }
    else
    {
        S32 str_width, str_height, width, height;
        color_t *fg;
        color_t bg;//092106 popup countdown

        fg = current_MMI_theme->popup_description_text_color;
        bg = wgui_pop_up_dialog_background.c;

        gui_set_font(&MMI_medium_font);
        //gui_set_text_color(UI_COLOR_BLACK);//092106 popup countdown
        gui_set_text_color(*fg);
        gui_itoa(Cat165CountDownValue, (UI_string_type) buf, 10);
        gdi_layer_push_clip();
        gdi_layer_reset_clip();
        gui_measure_string((UI_string_type) buf, &str_width, &str_height);
        width = MMI_pop_up_dialog_width - 5 - str_width;
    #ifdef __MMI_MAINLCD_128X128__//061306 
        height = MMI_pop_up_dialog_height - 7 - str_height;
    #else
        height = MMI_pop_up_dialog_height - 10 - str_height;
    #endif
    #if 0//092106 popup countdown
    #else
        gdi_draw_solid_rect(
            MMI_pop_up_dialog_x + 10,
            MMI_pop_up_dialog_y + height,
            MMI_pop_up_dialog_x + MMI_pop_up_dialog_width - 10,
            MMI_pop_up_dialog_y + height + str_height,
            gdi_act_color_from_rgb(bg.alpha, bg.r, bg.g, bg.b));
    #endif
        if (r2lMMIFlag)
        {
            gui_move_text_cursor(
                MMI_pop_up_dialog_x + ((MMI_pop_up_dialog_width + str_width) >> 1),
                MMI_pop_up_dialog_y + height);
        }
        else
        {
            gui_move_text_cursor(
                MMI_pop_up_dialog_x + ((MMI_pop_up_dialog_width - str_width) >> 1),
                MMI_pop_up_dialog_y + height);
        }
        gui_print_text((UI_string_type) buf);
        gdi_layer_pop_clip();
        gui_BLT_double_buffer(
            MMI_pop_up_dialog_x + 10,
            MMI_pop_up_dialog_y + height,
            MMI_pop_up_dialog_x + MMI_pop_up_dialog_width - 10,
            MMI_pop_up_dialog_y + height + str_height);
        gui_set_font(saved_font);
        gui_set_text_color(saved_color);
        Cat165CountDownValue--;
        gui_start_timer(1000, Cat165CountDownCallBack);
    }
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory165Screen_ext
 * DESCRIPTION
 *  Displays the category65 screen
 * PARAMETERS
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        Message string
 *  message_icon            [IN]        
 *  duration                [IN]        
 *  history_buffer          [IN]        History buffer
 *  UI_image_ID_type (?)          [IN]        Icon        Message icon
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory165Screen_ext(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        UI_string_type message,
        U16 message_icon,
        S32 duration,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
#endif 
     

    ShowCommonCategoryPopupScreenType2(
        0,
        0,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        (UI_string_type) message,
        history_buffer);
    ExitCategoryFunction = ExitCategory165Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY164_ID;
    dm_data.S32flags = DM_NO_TITLE | DM_NO_STATUS_BAR;
    dm_setup_data(&dm_data);
    dm_add_image(get_image(message_icon), NULL, NULL);
    dm_redraw_category_screen();
    Cat165CountDownValue = duration;
    if (Cat165CountDownValue != 0)
    {
        Cat165CountDownCallBack();
    }
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory165Screen
 * DESCRIPTION
 *  Displays the category65 screen
 * PARAMETERS
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  message                 [IN]        Message string
 *  message_icon            [IN]        
 *  history_buffer          [IN]        History buffer
 *  UI_image_ID_type (?)          [IN]        Icon        Message icon
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory165Screen(
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        UI_string_type message,
        U16 message_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowCategory165Screen_ext(
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        message,
        message_icon,
        0,
        history_buffer);
}

 


/*****************************************************************************
 * FUNCTION
 *  ExitCategory165Screen
 * DESCRIPTION
 *  Exits the category65 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory165Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    reset_small_screen();
#endif 
     

    gui_hide_animations();
    gui_cancel_timer(Cat165CountDownCallBack);
    Cat165TimerExpireCallBack = UI_dummy_function;
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}

#ifdef __MMI_TOUCH_SCREEN__
extern void DTGetRTCTime(MYTIME *t);


/*****************************************************************************
 * FUNCTION
 *  DrawCate264CategoryControlArea
 * DESCRIPTION
 *  
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate264CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    //      MYTIME  t;
    //      U8 ran;
    S32 img_width, img_height;
    S32 msg_width, msg_height;
    S32 pop_x = coordinate->S16X;
    S32 pop_y = coordinate->S16Y;
    S32 pop_width = coordinate->S16Width;
    S32 pop_height = coordinate->S16Height;
    extern S32 wgui_image_clip_x1;
    extern S32 wgui_image_clip_y1;
    extern S32 wgui_image_clip_x2;
    extern S32 wgui_image_clip_y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_image_get_dimension((U8*) MMI_message_icon, &img_width, &img_height);
    resize_multiline_inputbox_fit((pop_width >> 1), (pop_height >> 1), &msg_width, &msg_height);

    wgui_image_clip_x1 = pop_x + ((pop_width - img_width) >> 1);
    wgui_image_clip_y1 = pop_y + ((pop_height - img_height) >> 1);
    wgui_image_clip_x2 = wgui_image_clip_x1 + img_width;
    wgui_image_clip_y2 = wgui_image_clip_y1 + img_height;
    gdi_image_draw(wgui_image_clip_x1, wgui_image_clip_y1, (U8*) MMI_message_icon);
    move_multiline_inputbox(pop_x + ((pop_width - msg_width) >> 1), pop_y + ((pop_height + img_height) >> 1));
    show_multiline_inputbox();

     
    /*
     * DTGetRTCTime(&t);
     * ran=t.nSec%5;
     * switch (ran)
     * {
     * case 0://center
     * wgui_image_clip_x1 = pop_x+((pop_width-img_width)>>1);
     * wgui_image_clip_y1 = pop_y+((pop_height-img_height)>>1);
     * wgui_image_clip_x2 = wgui_image_clip_x1 + img_width;
     * wgui_image_clip_y2 = wgui_image_clip_y1 + img_height;
     * gdi_image_draw(wgui_image_clip_x1,wgui_image_clip_y1,(U8*)MMI_message_icon);
     * move_multiline_inputbox(pop_x+((pop_width-msg_width)>>1), pop_y+((pop_height+img_height)>>1));
     * show_multiline_inputbox();
     * break;
     * 
     * case 1://Left-Top
     * wgui_image_clip_x1 = pop_x+5;
     * wgui_image_clip_y1 = pop_y+5;
     * wgui_image_clip_x2 = wgui_image_clip_x1 + img_width;
     * wgui_image_clip_y2 = wgui_image_clip_y1 + img_height;
     * gdi_image_draw(wgui_image_clip_x1,wgui_image_clip_y1,(U8*)MMI_message_icon);
     * move_multiline_inputbox(pop_x+5, pop_y+10+img_height);
     * show_multiline_inputbox();
     * break;
     * 
     * case 2://Left-Bottom
     * wgui_image_clip_x1 = pop_x+5;
     * wgui_image_clip_y1 = pop_y+pop_height-img_height-5;
     * wgui_image_clip_x2 = wgui_image_clip_x1 + img_width;
     * wgui_image_clip_y2 = wgui_image_clip_y1 + img_height;
     * gdi_image_draw(wgui_image_clip_x1, wgui_image_clip_y1, (U8*)MMI_message_icon);
     * move_multiline_inputbox(pop_x+5, pop_y+pop_height-5-img_height-msg_height);
     * show_multiline_inputbox();
     * break;
     * 
     * case 3://Right-Top
     * wgui_image_clip_x1 = pop_x+pop_width-img_width-5;
     * wgui_image_clip_y1 = pop_y+5;
     * wgui_image_clip_x2 = wgui_image_clip_x1 + img_width;
     * wgui_image_clip_y2 = wgui_image_clip_y1 + img_height;
     * gdi_image_draw(wgui_image_clip_x1, wgui_image_clip_y1, (U8*)MMI_message_icon);
     * move_multiline_inputbox(pop_x+pop_width-msg_width-5, pop_y+5+img_height);
     * show_multiline_inputbox();
     * break;
     * 
     * case 4://Right-Bottom
     * wgui_image_clip_x1 = pop_x+pop_width-img_width-5;
     * wgui_image_clip_y1 = pop_y+pop_height-img_height-5;
     * wgui_image_clip_x2 = wgui_image_clip_x1 + img_width;
     * wgui_image_clip_y2 = wgui_image_clip_y1 + img_height;
     * gdi_image_draw(wgui_image_clip_x1, wgui_image_clip_y1, (U8*)MMI_message_icon);
     * move_multiline_inputbox(pop_x+pop_width-msg_width-5, pop_y+pop_height-5-img_height-msg_height);
     * show_multiline_inputbox();
     * break;
     * 
     * }
     */
     
}


/*****************************************************************************
 * FUNCTION
 *  category264_pen_down_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL category264_pen_down_handler(mmi_pen_point_struct point)
{
    #if 0
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 pos_x = point.x, pos_y = point.y;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_keylock_context.gKeyPadLockFlag &&
        (pos_x > wgui_image_clip_x1 && pos_x < wgui_image_clip_x2 && pos_y > wgui_image_clip_y1 &&
         pos_y < wgui_image_clip_y2))
    {
        HandlePoundKeyForUnlock();
        return MMI_TRUE;
    }
    return MMI_FALSE;
   #else
       extern U16 gKeyPadLockFlag ;
	S32 pos_x=point.x,pos_y=point.y;

	if ( gKeyPadLockFlag && (pos_x > wgui_image_clip_x1 && pos_x < wgui_image_clip_x2 && pos_y > wgui_image_clip_y1 && pos_y < wgui_image_clip_y2) )
	{
		HandlePoundKeyForUnlock();
		return MMI_TRUE;
	}
	return MMI_FALSE;
   #endif
}
extern void ShowMakeSOSCallScr(void);


/*****************************************************************************
 * FUNCTION
 *  Category264_image_pen_down_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL Category264_image_pen_down_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(point);
    ShowMakeSOSCallScr();
    return MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory264Screen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  message             [IN]        
 *  message_icon        [IN]        
 *  para                [?]         
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory264Screen(UI_string_type message, PU8 message_icon, U16 dial_icon, void *para, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    set_small_screen();
#endif 
     

    ShowCommonCategoryPopupScreen(message, history_buffer);
    MMI_message_icon = message_icon;
    MMI_message_string = message;
    dm_register_category_controlled_callback(DrawCate264CategoryControlArea);
    wgui_register_category_screen_control_area_pen_handlers(category264_pen_down_handler, MMI_PEN_EVENT_DOWN);
    if ( dial_icon == 0 )
    {
        dm_add_image(NULL,NULL,NULL );
    }
    else
    {
/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-10 11:13 */
#if (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_WGUI_CSK_ENABLE__)    
		dm_add_image(get_image(IMG_CSK_DIAL_ICON),NULL,NULL );
#endif
/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-10 11:13 */
        wgui_register_image_control_pen_handlers(0, Category264_image_pen_down_handler, MMI_PEN_EVENT_DOWN);
    }

    ExitCategoryFunction = ExitCategory62Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY264_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}
#endif /* __MMI_TOUCH_SCREEN__ */ 

