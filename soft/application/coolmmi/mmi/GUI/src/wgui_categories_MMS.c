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
 *  wgui_categories_MMS.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  MMS related categories. 
 *  [Category275]       MMS SMIL editor
 *  Note: Most MMS screens are located in mcu\wapadp\msf_ui\ instead of here.
 * Author:
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"

/* framework related include */
#include "eventsgprot.h"
#include "globaldefs.h"
#include "frameworkstruct.h"
#include "eventsdef.h"
#include "unicodexdcl.h"
#include "debuginitdef.h"

/* gui related include */
#include "gui_themes.h"
#include "wgui.h"
#include "wgui_softkeys.h"
#include "wgui_categories.h"
#include "wgui_categories_defs.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_mms.h"
#include "wgui_draw_manager.h"
#include "wgui_status_icons.h"
#if defined(__MMI_TOUCH_SCREEN__)
#include "editorpen.h"
#endif 

#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
#endif 

#undef __NEWSIMULATOR

#if defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
#endif /* defined(__MMI_SMART_MESSAGE_MT__) || (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) */ 

#ifdef __MMI_T9__
#include "t9main.h"     /* t9synt9withinputbox() */
extern void InuptMethodEnterCategory5(void);
#elif defined __MMI_ZI__
extern void ZiInuptMethodEnterCategory5(void);
#elif defined __MMI_KA__
extern void KonkaInuptMethodEnterCategory5(void);
#elif defined __MMI_ITAP__
extern void ItapInuptMethodEnterCategory5(void);
#endif 
#ifdef __MMI_GB__
extern void GbInuptMethodEnterCategory5(void);
#endif
extern void dm_set_scr_bg_image_no_draw(U16 image_id, S8 *file_name, S32 x, S32 y, U8 opacity);         

#ifdef __MMI_MMS__
/***************************************************************************** 
* Define
*****************************************************************************/
#if defined(__MMI_MAINLCD_240X320__)
#define CAT275_MIN_LINE_HEIGHT         (26)
#elif defined(__MMI_MAINLCD_176X220__)
#define CAT275_MIN_LINE_HEIGHT         (20)
#elif defined(__MMI_MAINLCD_220X176__)
#define CAT275_MIN_LINE_HEIGHT         (20)
#elif defined(__MMI_MAINLCD_128X160__)
#define CAT275_MIN_LINE_HEIGHT         (19)
#elif defined(__MMI_MAINLCD_128X128__)
#define CAT275_MIN_LINE_HEIGHT         (19)
#elif defined(__MMI_MAINLCD_160X128__)
#define CAT275_MIN_LINE_HEIGHT         (19)
#else 
/* However, category 275 should not be used on 128x128 display */
#define CAT275_MIN_LINE_HEIGHT         (19)
#endif 

/***************************************************************************** 
* Typedef 
*****************************************************************************/

typedef struct
{
    U32 identifier;
    wgui_cat275_draw_title_hdlr draw_title_fp;
    wgui_cat275_draw_area_hdlr draw_area_1_fp;
    wgui_cat275_draw_area_hdlr draw_area_2_fp;
    S16 draw_area_1_height;
    S16 draw_area_2_height;
    UI_filled_area *backup_normal_filler;   /* normal filler of editor */
    UI_filled_area *backup_multitap_filler;
} gui_cat275_context_struct;

/***************************************************************************** 
* Local Variable
*****************************************************************************/

static gui_cat275_context_struct *g_mmi_gui_cat275_context_p;

static BOOL g_mmi_gui_cat275_modified;

#define CAT275_CTX         (g_mmi_gui_cat275_context_p)

static UI_filled_area g_mmi_gui_cat275_editor_filler = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_BORDER,
    NULL,
    NULL,
    {255, 255, 255, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    0
};

static UI_filled_area g_mmi_gui_cat275_multitap_filler = {UI_FILLED_AREA_TYPE_COLOR,
    NULL,
    NULL,
    {255, 255, 255, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    0
};

#if defined (__MMI_WALLPAPER_ON_BOTTOM__)
static UI_image_ID_type  editor_scr_bg_ID;
static S8 *editor_scr_bg_filename;
static S32 editor_scr_bg_x, editor_scr_bg_y;
static U8 editor_scr_bg_opacity;
#endif /* defined (__MMI_WALLPAPER_ON_BOTTOM__) */ 

/***************************************************************************** 
* Local Function
*****************************************************************************/

/*****************************************************************************
 * FUNCTION
 *  category275_hide_multitap
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category275_hide_multitap(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* We fill the multitap bar across the screen instead of using MMI_multitap_x & MMI_multitap_width */
    color_t c;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();

#ifdef __MMI_DEFAULT_THEME_3__
    c.r = 206;
    c.g = 206;
    c.b = 206;
#else /* __MMI_DEFAULT_THEME_3__ */ 
    c.r = 255;
    c.g = 255;
    c.b = 255;
#endif /* __MMI_DEFAULT_THEME_3__ */ 
    c.alpha = 100;
     

    gui_reset_clip();
     
    gui_fill_rectangle(0, MMI_multitap_y, UI_device_width - 1, MMI_multitap_y + MMI_multitap_height - 1, c);

    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, MMI_multitap_y, UI_device_width - 1, MMI_multitap_y + MMI_multitap_height + 1);
}


/*****************************************************************************
 * FUNCTION
 *  category275_header_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  yoffset     [IN]        
 *  clip_x1     [IN]        
 *  clip_y1     [IN]        
 *  clip_x2     [IN]        
 *  clip_y2     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void category275_header_callback(S32 yoffset, S32 clip_x1, S32 clip_y1, S32 clip_x2, S32 clip_y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (CAT275_CTX)
    {
        CAT275_CTX->draw_area_1_fp(CAT275_CTX->identifier, yoffset, clip_x1, clip_y1, clip_x2, clip_y2);
    }
}


/*****************************************************************************
 * FUNCTION
 *  category275_footer_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  yoffset     [IN]        
 *  clip_x1     [IN]        
 *  clip_y1     [IN]        
 *  clip_x2     [IN]        
 *  clip_y2     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void category275_footer_callback(S32 yoffset, S32 clip_x1, S32 clip_y1, S32 clip_x2, S32 clip_y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (CAT275_CTX)
    {
        CAT275_CTX->draw_area_2_fp(CAT275_CTX->identifier, yoffset, clip_x1, clip_y1, clip_x2, clip_y2);
    }
}


/*****************************************************************************
 * FUNCTION
 *  category275_change_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category275_change_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_gui_cat275_modified = TRUE;
}

/* We need to sync with T9 internal state */


/*****************************************************************************
 * FUNCTION
 *  category275_sync_with_T9
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category275_sync_with_T9(void)
{
#ifdef __MMI_T9__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (MMI_current_input_type & INPUT_TYPE_MASK)
    {
        case INPUT_TYPE_TR_BOPOMO:
        case INPUT_TYPE_SM_PINYIN:
        case INPUT_TYPE_TR_STROKE:
        case INPUT_TYPE_SM_STROKE:
        case INPUT_TYPE_SMART_UPPERCASE_ABC:
        case INPUT_TYPE_SMART_LOWERCASE_ABC:
        case INPUT_TYPE_TR_MULTITAP_BOPOMO:
        case INPUT_TYPE_SM_MULTITAP_PINYIN:

    #if defined(__MMI_T9_SPANISH__)
        case INPUT_TYPE_SMART_UPPERCASE_SPANISH:
        case INPUT_TYPE_SMART_LOWERCASE_SPANISH:
    #endif /* defined(__MMI_T9_SPANISH__) */ 
             
    #if defined(__MMI_T9_DANISH__)
        case INPUT_TYPE_SMART_UPPERCASE_DANISH:
        case INPUT_TYPE_SMART_LOWERCASE_DANISH:
    #endif /* defined(__MMI_T9_DANISH__) */ 
             
             
    #if defined(__MMI_T9_POLISH__)
        case INPUT_TYPE_SMART_UPPERCASE_POLISH:
        case INPUT_TYPE_SMART_LOWERCASE_POLISH:
    #endif /* defined(__MMI_T9_POLISH__) */ 
             
    #if defined(__MMI_T9_FRENCH__)
        case INPUT_TYPE_SMART_UPPERCASE_FRENCH:
        case INPUT_TYPE_SMART_LOWERCASE_FRENCH:
    #endif /* defined(__MMI_T9_FRENCH__) */ 
    #if defined(__MMI_T9_GERMAN__)
        case INPUT_TYPE_SMART_UPPERCASE_GERMAN:
        case INPUT_TYPE_SMART_LOWERCASE_GERMAN:
    #endif /* defined(__MMI_T9_GERMAN__) */ 
    #if defined(__MMI_T9_ITALIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_ITALIAN:
        case INPUT_TYPE_SMART_LOWERCASE_ITALIAN:
    #endif /* defined(__MMI_T9_ITALIAN__) */ 

             

    #if defined(__MMI_T9_VIETNAMESE__)
        case INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
        case INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
    #endif /* defined(__MMI_T9_VIETNAMESE__) */ 

             

             
    #if defined(__MMI_T9_INDONESIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_INDONESIAN:
        case INPUT_TYPE_SMART_LOWERCASE_INDONESIAN:
    #endif /* defined(__MMI_T9_INDONESIAN__) */ 
             
             
    #if defined(__MMI_T9_CZECH__)
        case INPUT_TYPE_SMART_UPPERCASE_CZECH:
        case INPUT_TYPE_SMART_LOWERCASE_CZECH:
    #endif /* defined(__MMI_T9_CZECH__) */ 
             
             
    #if defined(__MMI_T9_TURKISH__)
        case INPUT_TYPE_SMART_UPPERCASE_TURKISH:
        case INPUT_TYPE_SMART_LOWERCASE_TURKISH:
    #endif /* defined(__MMI_T9_TURKISH__) */ 
             
             
    #if defined(__MMI_T9_PORTUGUESE__)
        case INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE:
        case INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
    #endif /* defined(__MMI_T9_PORTUGUESE__) */ 
             
             
    #if defined(__MMI_T9_FINNISH__)
        case INPUT_TYPE_SMART_UPPERCASE_FINNISH:
        case INPUT_TYPE_SMART_LOWERCASE_FINNISH:
    #endif /* defined(__MMI_T9_FINNISH__) */ 
    #if defined(__MMI_T9_HUNGARIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_HUNGARIAN:
        case INPUT_TYPE_SMART_LOWERCASE_HUNGARIAN:
    #endif /* defined(__MMI_T9_HUNGARIAN__) */ 
             
 
    #if defined(__MMI_T9_ARABIC__)
        case INPUT_TYPE_SMART_ARABIC:
    #endif 
             
    #if defined(__MMI_T9_HINDI__)
        case INPUT_TYPE_SMART_HINDI:
    #endif 
             
            //Norwegian start
    #if defined(__MMI_T9_NORWEGIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_NORWEGIAN:
        case INPUT_TYPE_SMART_LOWERCASE_NORWEGIAN:
    #endif /* defined(__MMI_T9_NORWEGIAN__) */ 
            //Norwegian end
            //START DUTCH_T9 20051021
    #if defined(__MMI_T9_DUTCH__)
        case INPUT_TYPE_SMART_UPPERCASE_DUTCH:
        case INPUT_TYPE_SMART_LOWERCASE_DUTCH:
    #endif /* defined(__MMI_T9_DUTCH__) */ 
            //END DUTCH_T9 20051021
 
  
    #if defined(__MMI_T9_MALAY__)
        case INPUT_TYPE_SMART_UPPERCASE_MALAY:
        case INPUT_TYPE_SMART_LOWERCASE_MALAY:
    #endif /* defined(__MMI_T9_MALAY__) */ 
    #if defined(__MMI_T9_CROATIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_CROATIAN:
        case INPUT_TYPE_SMART_LOWERCASE_CROATIAN:
    #endif /* defined(__MMI_T9_CROATIAN__) */ 
    #if defined(__MMI_T9_SLOVAK__)
        case INPUT_TYPE_SMART_UPPERCASE_SLOVAK:
        case INPUT_TYPE_SMART_LOWERCASE_SLOVAK:
    #endif /* defined(__MMI_T9_SLOVAK__) */ 
    #if defined(__MMI_T9_ROMANIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_ROMANIAN:
        case INPUT_TYPE_SMART_LOWERCASE_ROMANIAN:
    #endif /* defined(__MMI_T9_ROMANIAN__) */ 
 
            T9SynT9WithInputBox();
            break;
    }
#endif /* __MMI_T9__ */ 
}

/***************************************************************************** 
* Global Variable
*****************************************************************************/

/***************************************************************************** 
* Global Function
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  DrawCate275CategoryControlArea
 * DESCRIPTION
 *  
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void DrawCate275CategoryControlArea(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    show_title_status_icon();
    CAT275_CTX->draw_title_fp(CAT275_CTX->identifier);

    wgui_show_inputbox();
     
#if !defined(__MMI_FULL_SCREEN_EDITOR__)
    category275_hide_multitap();    /* For the first time display */
#endif 

    if (RedrawSpellingOrCandidateBoxesFunction)
    {
        RedrawSpellingOrCandidateBoxesFunction();
    }

}


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory275Screen
 * DESCRIPTION
 *  Redraws the category275 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory275Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();

    clear_screen();

    CAT275_CTX->draw_title_fp(CAT275_CTX->identifier);

#if defined(__MMI_FULL_SCREEN_EDITOR__)
    category275_hide_multitap();    /* For the first time display */
#endif 
    wgui_show_inputbox();
#if !defined(__MMI_FULL_SCREEN_EDITOR__)
    category275_hide_multitap();    /* For the first time display */
#endif 

    show_softkey_background();
    show_left_softkey();
    show_right_softkey();

    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);

    if (RedrawSpellingOrCandidateBoxesFunction)
    {
        RedrawSpellingOrCandidateBoxesFunction();
    }
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory275RightSoftkeyFunction
 * DESCRIPTION
 *  
 * PARAMETERS
 *  f       [IN]        
 *  k       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory275RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(k);
    wgui_inputbox_RSK_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  Category275GotoTop
 * DESCRIPTION
 *  Invoked after ShowCategory275Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void Category275GotoTop(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_multiline_inputbox.cursor_p = MMI_multiline_inputbox.text;

    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
    gui_show_multi_line_input_box(&MMI_multiline_inputbox);
    MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;

    category275_sync_with_T9();

    MMI_multiline_inputbox.text_offset_y = 0;
    gui_show_multi_line_input_box(&MMI_multiline_inputbox);
}


/*****************************************************************************
 * FUNCTION
 *  Category275GotoBottom
 * DESCRIPTION
 *  Invoked after ShowCategory275Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void Category275GotoBottom(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_multiline_inputbox.text_length >= 2)
    {
        MMI_multiline_inputbox.cursor_p =
            (UI_buffer_type) ((U8*) (MMI_multiline_inputbox.text) + (MMI_multiline_inputbox.text_length - 2));
    }
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
    gui_show_multi_line_input_box(&MMI_multiline_inputbox);
    MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;

    category275_sync_with_T9();

    if (MMI_multiline_inputbox.text_height > MMI_multiline_inputbox.edit_height)
    {
        MMI_multiline_inputbox.text_offset_y = MMI_multiline_inputbox.edit_height - MMI_multiline_inputbox.text_height;
    }
    gui_show_multi_line_input_box(&MMI_multiline_inputbox);
}


/*****************************************************************************
 * FUNCTION
 *  Category275SetUnmodified
 * DESCRIPTION
 *  Set that category 275 is unmodified.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void Category275SetUnmodified(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_gui_cat275_modified = FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  Category275CheckIfModified
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  Whether category 275 is modified.
 *****************************************************************************/
BOOL Category275CheckIfModified(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return g_mmi_gui_cat275_modified;
}


/*****************************************************************************
 * FUNCTION
 *  cat275_virtual_keypad_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat275_virtual_keypad_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(__MMI_TOUCH_SCREEN__)
    mmi_pen_editor_resize_multiline_input_box_for_vk();
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory275Screen
 * DESCRIPTION
 *  Displays the category275 screen
 * PARAMETERS
 *  identifier              [IN]        Used as parameter for the callback functions described before
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  input_type              [IN]        Type of input to use
 *  buffer                  [IN]        Buffer the input box should use.
 *  buffer_size             [IN]        Size of the buffer.
 *  vbar_width              [IN]        
 *  draw_title_fp           [IN]        
 *  draw_area_1_height      [IN]        
 *  draw_area_1_fp          [IN]        
 *  draw_area_2_height      [IN]        
 *  draw_area_2_fp          [IN]        
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory275Screen(
        U32 identifier,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S16 input_type,
        U8 *buffer,
        S32 buffer_size,
        U16 vbar_width,
        wgui_cat275_draw_title_hdlr draw_title_fp,
        U16 draw_area_1_height,
        wgui_cat275_draw_area_hdlr draw_area_1_fp,
        U16 draw_area_2_height,
        wgui_cat275_draw_area_hdlr draw_area_2_fp,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ShowCategory277Screen(
        identifier, 
        left_softkey, 
        left_softkey_icon, 
        right_softkey, 
        right_softkey_icon, 
        input_type, 
        buffer, 
        buffer_size, 
        vbar_width, 
        draw_title_fp, 
        draw_area_1_height,
        draw_area_1_fp,
        draw_area_2_height,
        draw_area_2_fp,
        NULL,
        NULL,
        NULL,
        history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory277Screen
 * DESCRIPTION
 *  Displays the category277 screen
 * PARAMETERS
 *  identifier              [IN]        Used as parameter for the callback functions described before
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  input_type              [IN]        Type of input to use
 *  buffer                  [IN]        Buffer the input box should use.
 *  buffer_size             [IN]        Size of the buffer.
 *  vbar_width              [IN]        
 *  draw_title_fp           [IN]        
 *  draw_area_1_height      [IN]        
 *  draw_area_1_fp          [IN]        
 *  draw_area_2_height      [IN]        
 *  draw_area_2_fp          [IN]        
 *  text_font               [IN]        
 *  text_color              [IN]        
 *  bg_color                [IN]        
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory277Screen(
        U32 identifier,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S16 input_type,
        U8 *buffer,
        S32 buffer_size,
        U16 vbar_width,
        wgui_cat275_draw_title_hdlr draw_title_fp,
        U16 draw_area_1_height,
        wgui_cat275_draw_area_hdlr draw_area_1_fp,
        U16 draw_area_2_height,
        wgui_cat275_draw_area_hdlr draw_area_2_fp,
        UI_font_type *text_font,
        color_t *text_color,
        color_t *bg_color,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 inputbox_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Setup context */
    g_mmi_gui_cat275_context_p = OslMalloc(sizeof(gui_cat275_context_struct));
    CAT275_CTX->identifier = identifier;
    CAT275_CTX->draw_title_fp = draw_title_fp;
    CAT275_CTX->draw_area_1_fp = draw_area_1_fp;
    CAT275_CTX->draw_area_2_fp = draw_area_2_fp;
    CAT275_CTX->draw_area_1_height = draw_area_1_height;
    CAT275_CTX->draw_area_2_height = draw_area_2_height;

    /* Setup layout */
    gui_lock_double_buffer();

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    /* Input box */
#ifdef __MMI_T9__
    InuptMethodEnterCategory5();
#elif defined __MMI_ZI__
    ZiInuptMethodEnterCategory5();
#elif defined __MMI_KA__
    KonkaInuptMethodEnterCategory5();
#elif defined __MMI_ITAP__
    ItapInuptMethodEnterCategory5();
#elif defined __MMI_GB__
    GbInuptMethodEnterCategory5();
#endif 

    register_hide_multitap(category275_hide_multitap);

    wgui_setup_inputbox_ext(0, MMI_content_y, MMI_content_width,
#if defined(__MMI_FULL_SCREEN_EDITOR__)
                            MMI_content_height,
#else 
                            MMI_content_height - MMI_multitap_height,
#endif 
                            buffer, buffer_size,
                            MMI_CATEGORY275_ID, get_string(right_softkey), get_image(right_softkey_icon),
                            input_type, history_buffer, 1, UI_MULTI_LINE_INPUT_BOX_DRAW_UNDERLINE,
#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__)||defined(__MMI_MAINLCD_220X176__)        // TODO: tune style 4
                            (S16) (draw_area_1_height + draw_area_2_height + CAT275_MIN_LINE_HEIGHT * 2),
#else 
                            (S16) (draw_area_1_height + draw_area_2_height + CAT275_MIN_LINE_HEIGHT),
#endif 
                            CAT275_MIN_LINE_HEIGHT, (S16) draw_area_1_height, (S16) draw_area_2_height, NULL);

#if defined(__MMI_FULL_SCREEN_EDITOR__)
    inputbox_height = MMI_content_height - wgui_inputbox_information_bar_height;
#else 
    inputbox_height = MMI_content_height - MMI_multitap_height - wgui_inputbox_information_bar_height;
#endif 
    if (MMI_multiline_inputbox.height > inputbox_height)
    {
        gui_resize_multi_line_input_box(&MMI_multiline_inputbox, MMI_multiline_inputbox.width, inputbox_height);
    }

    /* Input box attributes */
    MMI_multiline_inputbox.header_callback = category275_header_callback;
    MMI_multiline_inputbox.footer_callback = category275_footer_callback;
    MMI_multiline_inputbox.change_callback = category275_change_callback;

    /* Disable editor border and set background color of multitap */
    CAT275_CTX->backup_normal_filler = MMI_multiline_inputbox.normal_filler;

    if (MMI_multiline_inputbox.normal_filler)
    {
        if ((MMI_multiline_inputbox.normal_filler->flags & UI_FILLED_AREA_MASK_TYPE) == UI_FILLED_AREA_TYPE_COLOR)
        {
            g_mmi_gui_cat275_editor_filler.flags = UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_BORDER;
            if (bg_color != NULL)
            {
                g_mmi_gui_cat275_editor_filler.c = *bg_color;
            }
            else
            {
                g_mmi_gui_cat275_editor_filler.c = MMI_multiline_inputbox.normal_filler->c;
            }
            
            /* Tricky. Use multitap text color as editor border color */
            g_mmi_gui_cat275_editor_filler.border_color = *current_MMI_theme->multitap_normal_text_color;

            MMI_multiline_inputbox.normal_filler = (UI_filled_area*)&g_mmi_gui_cat275_editor_filler;       
        }
   }

    if (text_color != NULL)
    {
        MMI_multiline_inputbox.normal_text_color = *text_color;
    }

    if (text_font != NULL)
    {
        MMI_multiline_inputbox.text_font = *text_font;
    }

    CAT275_CTX->backup_multitap_filler = current_multitap_input_theme->f;
    g_mmi_gui_cat275_multitap_filler = *(current_multitap_input_theme->f);
    current_multitap_input_theme->f = (UI_filled_area*)&g_mmi_gui_cat275_multitap_filler;
    set_MMI_multitap_theme();

     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
    dm_get_scr_bg_image(
        &editor_scr_bg_ID,
        &editor_scr_bg_filename,
        &editor_scr_bg_x,
        &editor_scr_bg_y,
        &editor_scr_bg_opacity);
    dm_set_scr_bg_image(editor_scr_bg_ID, editor_scr_bg_filename, editor_scr_bg_x, editor_scr_bg_y, MMI_EDITOR_BG_OPACITY);      
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
     

    gui_unlock_double_buffer();

    /* Setup standard category handler */
    ExitCategoryFunction = ExitCategory275Screen;
    dm_setup_category_functions(dm_redraw_category_screen, GetCategory275History, GetCategory275HistorySize);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY275_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_data.S32flags |= DM_SHOW_VKPAD;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(DrawCate275CategoryControlArea);
    dm_register_vkpad_callback(cat275_virtual_keypad_callback);
    dm_redraw_category_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory275Screen
 * DESCRIPTION
 *  Exits the category275 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory275Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Reset UI */
    set_MMI_current_input_type();
    reset_softkeys();
    reset_multitaps();
    wgui_close_inputbox();

     
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
     
    dm_set_scr_bg_image_no_draw(
        editor_scr_bg_ID,
        editor_scr_bg_filename,
        editor_scr_bg_x,
        editor_scr_bg_y,
        editor_scr_bg_opacity);
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
     

    /*
     * Release context after other deinitialization routine
     * * because they might refer to CAT275_CTX
     */

    MMI_multiline_inputbox.normal_filler = CAT275_CTX->backup_normal_filler;
    current_multitap_input_theme->f = CAT275_CTX->backup_multitap_filler;
    set_MMI_multitap_theme();

    OslMfree(g_mmi_gui_cat275_context_p);
    g_mmi_gui_cat275_context_p = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory275HistorySize
 * DESCRIPTION
 *  Gets the size of the history buffer for category275 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  size in U8s of the history buffer
 *****************************************************************************/
S32 GetCategory275HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return sizeof(multiline_inputbox_category_history);
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory275History
 * DESCRIPTION
 *  Gets the history buffer for category275 screen
 * PARAMETERS
 *  history_buffer      [IN]        Is the buffer into which the history data is stored (pre-allocated)
 * RETURNS
 *  Pointer to the history buffer
 *****************************************************************************/
U8 *GetCategory275History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_multiline_inputbox_category_history(MMI_CATEGORY275_ID, history_buffer, MMI_current_input_type);
    return history_buffer;
}

#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__) || defined(__MMI_SMART_MESSAGE_MT__)
#if defined(__MMI_TOUCH_SCREEN__)
#endif 
#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
#endif /* defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__) */ 
#ifdef __MMI_T9__
#elif defined __MMI_ZI__
#elif defined __MMI_KA__
#elif defined __MMI_ITAP__
#endif 
#if defined(__MMI_FULL_SCREEN_EDITOR__)
#else 
#endif 
#if defined(__MMI_FULL_SCREEN_EDITOR__)
#else 
#endif 
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
#endif /* defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__) */ 
#if defined(__MMI_SMART_MESSAGE_MT__)
#endif /* defined(__MMI_SMART_MESSAGE_MT__) */ 
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 
#if 0
#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
#endif
#if defined(__MMI_SMART_MESSAGE_MT__)
#endif
#endif /* 0 */ 
#if defined(__MMI_FULL_SCREEN_EDITOR__)
#endif 
#if !defined(__MMI_FULL_SCREEN_EDITOR__)
#endif 
#endif /* defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__) || defined(__MMI_SMART_MESSAGE_MT__) */ 

/* deepali view screen */
#define CAT276_SLIDING_BAR_H_SPACE     8
#define CAT276_SLIDING_BAR_V_SPACE     4


/*****************************************************************************
 * FUNCTION
 *  RedrawCategory276Screen
 * DESCRIPTION
 *  redraw category screen 276
 * PARAMETERS
 *  void
 * RETURNS
 *  size in U8s of the history buffer(?)
 *****************************************************************************/
void RedrawCategory276Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    clear_screen();
    /* MMI_title_y=18; */
    draw_title();
    show_softkey_background();
    show_left_softkey();
    show_right_softkey();
    /* DrawCat276SlidingBar(0); */
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory276Screen
 * DESCRIPTION
 *  Show category 276
 * PARAMETERS
 *  title                   [?]         
 *  title_icon              [IN]        
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 *  history_buffer          [?]         
 * RETURNS
 *  size in U8s of the history buffer(?)
 *****************************************************************************/
void ShowCategory276Screen(
        U8 *title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* gui_lock_double_buffer(); */
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_disable_title_shortcut_display = 1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    //      entry_full_screen();
    //      gOnFullScreen=MMI_IN_FULL_SCREEN;
    if (gOnFullScreen & MMI_LEAVE_FULL_SCREEN)
    {
        gOnFullScreen = MMI_IN_NORMAL_SCREEN;
    }

    hide_status_icon_bar(0);
    hide_status_icon_bar(1);
    MMI_title_icon = (PU8) get_image(title_icon);
    MMI_title_string = (UI_string_type) title;

    ExitCategoryFunction = ExitCategory276Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history, dummy_get_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY276_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_register_category_controlled_callback(dm_category_276_controlled_area);
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  dm_category_276_controlled_area
 * DESCRIPTION
 *  This function is used to draw the category controlled area of category screen
 * PARAMETERS
 *  coordinate      [IN/OUT]        Coordinates of category controlled area.
 * RETURNS
 *  void
 *****************************************************************************/
void dm_category_276_controlled_area(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    RedrawCategory276Screen();
}


/*****************************************************************************
 * FUNCTION
 *  DrawCat276Title
 * DESCRIPTION
 *  Draw category 276 title
 * PARAMETERS
 *  title       [?]     
 * RETURNS
 *  size in U8s of the history buffer(?)
 *****************************************************************************/
void DrawCat276Title(U8 *title)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    MMI_title_string = (UI_string_type) title;
    draw_title();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);

}


/*****************************************************************************
 * FUNCTION
 *  DrawCat276SlidingBar
 * DESCRIPTION
 *  Draw category 276 sliding bar.
 * PARAMETERS
 *  percentage      [IN]        
 * RETURNS
 *  size in U8s of the history buffer(?)
 *****************************************************************************/
void DrawCat276SlidingBar(U8 percentage)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, x3;
    color_t bg_color, *bar_border_color, *bar_filler_color, *bar_unfill_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x1 = 0;
    y1 = 0;
    x2 = UI_device_width - 1;
    y2 = MMI_title_y - 1;

    gui_lock_double_buffer();

    gui_push_clip();
    gui_set_clip(x1, y1, x2, y2);

    bg_color = current_MMI_theme->title_filler->c;

    gui_fill_rectangle(x1, y1, x2, y2, gui_color(bg_color.r, bg_color.g, bg_color.b));

    x1 += CAT276_SLIDING_BAR_H_SPACE;
    y1 += CAT276_SLIDING_BAR_V_SPACE;
    x2 -= CAT276_SLIDING_BAR_H_SPACE;
    y2 -= CAT276_SLIDING_BAR_V_SPACE;

    bar_border_color = current_MMI_theme->UI_black_color;

    gui_line(x1, y1 + 1, x1, y2 - 1, *bar_border_color);
    gui_line(x2, y1 + 1, x2, y2 - 1, *bar_border_color);
    gui_line(x1 + 1, y1, x2 - 1, y1, *bar_border_color);
    gui_line(x1 + 1, y2, x2 - 1, y2, *bar_border_color);

    x1++;
    y1++;
    x2--;
    y2--;

    if (percentage > 100)
    {
        percentage = 100;
    }

    x3 = x1 + (((x2 - x1) * percentage) / 100);

    bar_filler_color = current_MMI_theme->title_text_color;
    bar_unfill_color = current_MMI_theme->UI_white_color;

    gui_fill_rectangle(x1, y1, x3, y2, *bar_filler_color);

    gui_fill_rectangle(x3 + 1, y1, x2, y2, *bar_unfill_color);

    gui_pop_clip();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory276Screen
 * DESCRIPTION
 *  Exit category276 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  size in U8s of the history buffer(?)
 *****************************************************************************/
void ExitCategory276Screen(void)
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
    leave_full_screen();
}
#endif

