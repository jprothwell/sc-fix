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
 *   gui_generic_menuitems.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Generic menu items - UI component
 *   (deprecated & unused)
 * Author:
 * -------
 * -------
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
   Filename:      gui_generic_menuitems.h
   Date Created:  August-13-2002
   Contains:      UI routines header for Generic Menuitems
**********************************************************************************/

#ifndef __GUI_GENERIC_MENUITEMS_H__
#define  __GUI_GENERIC_MENUITEMS_H__

#include "gui.h"
 
#include "gui_theme_struct.h"
 
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

    /* UI simple text menu item structure           */
    extern UI_text_menuitem_theme *current_text_menuitem_theme;

    typedef struct _text_menuitem
    {
        S32 x, y;
        S32 width, height;
        UI_filled_area *normal_filler;
        UI_filled_area *disabled_filler;
        UI_filled_area *selected_filler;
        UI_filled_area *focussed_filler;
        U32 flags;
        S32 text_x, text_y;
        UI_string_type text;
        color_t normal_text_color;
        color_t disabled_text_color;
        color_t selected_text_color;
        color_t focussed_text_color;
        UI_font_type text_font;
    } text_menuitem;

    extern void gui_set_text_menuitem_current_theme(text_menuitem *m);
    extern void gui_set_text_menuitem_theme(text_menuitem *m, UI_text_menuitem_theme *t);
    extern void gui_create_text_menuitem(text_menuitem *m, S32 x, S32 y, S32 width, S32 height, UI_string_type text);
    extern void gui_text_menuitem_set_text_position(text_menuitem *m, S32 x, S32 y);
    extern void gui_show_text_menuitem(text_menuitem *m);
    extern void gui_show_text_menuitem_xy(text_menuitem *m, S32 x, S32 y);
    extern void gui_vshow_text_menuitem(void *m, S32 x, S32 y);
    extern void gui_measure_text_menuitem(void *m, S32 *x, S32 *y, S32 *width, S32 *height);
    extern void gui_highlight_text_menuitem(void *m);
    extern void gui_remove_highlight_text_menuitem(void *m);

    /* UI icontext menuitem structure         */
    extern UI_icontext_menuitem_theme *current_icontext_menuitem_theme;

    typedef struct _icontext_menuitem
    {
        S32 x, y;
        S32 width, height;
        UI_filled_area *normal_filler;
        UI_filled_area *disabled_filler;
        UI_filled_area *selected_filler;
        UI_filled_area *focussed_filler;
        U32 flags;
        S32 text_x, text_y;
        UI_string_type text;
        color_t normal_text_color;
        color_t disabled_text_color;
        color_t selected_text_color;
        color_t focussed_text_color;
        UI_font_type text_font;
        PU8 item_icon;
        S32 icon_x, icon_y;
        UI_transparent_color_type icon_transparent_color;
    } icontext_menuitem;

    extern void gui_set_icontext_menuitem_current_theme(icontext_menuitem *m);
    extern void gui_set_icontext_menuitem_theme(icontext_menuitem *m, UI_icontext_menuitem_theme *t);
    extern void gui_create_icontext_menuitem(
                    icontext_menuitem *m,
                    S32 x,
                    S32 y,
                    S32 width,
                    S32 height,
                    UI_string_type text,
                    PU8 item_icon);
    extern void gui_resize_icontext_menuitem(icontext_menuitem *m, S32 width, S32 height);
    extern void gui_icontext_menuitem_set_text_position(icontext_menuitem *m, S32 x, S32 y);
    extern void gui_icontext_menuitem_set_icon_position(icontext_menuitem *m, S32 x, S32 y);
    extern void gui_icontext_menuitem_set_icon(
                    icontext_menuitem *m,
                    PU8 item_icon,
                    UI_transparent_color_type icon_transparent_color);
    extern void gui_show_icontext_menuitem(icontext_menuitem *m);
    extern void gui_show_icontext_menuitem_xy(icontext_menuitem *m, S32 x, S32 y);
    extern void gui_vshow_icontext_menuitem(void *m, S32 x, S32 y);
    extern void gui_measure_icontext_menuitem(void *m, S32 *x, S32 *y, S32 *width, S32 *height);
    extern void gui_highlight_icontext_menuitem(void *m);
    extern void gui_remove_highlight_icontext_menuitem(void *m);

#ifdef __cplusplus
}
#endif 

#endif /* __GUI_GENERIC_MENUITEMS_H__ */ 

