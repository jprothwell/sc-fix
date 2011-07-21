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
*  wgui_dynamic_menus.c
*
* Project:
* --------
*   PlutoMMI
*
* Description:
* ------------
*   In order to use limited memory (size decided in compile-time) to load unlimited items (size decide in run-time) 
*   to a category screen, a modification in MMI for dynamic item loading is required as the following.
*
* Author:
* -------
 
*
*******************************************************************************/

#include "custdatares.h"
#include "wgui_fixed_menus.h"
#include "wgui_dynamic_menus.h"
#include "wgui_categories.h"
#include "gui_custom_menus.h"

extern void dummy_highlight_handler(S32 item_index);
extern void (*_MMI_hide_fixed_list_menu) (void);
extern void wgui_display_fixed_text_list_pop_up_description(void);
extern void wgui_display_dynamic_text_list_pop_up_description(void);

#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_LIST_MENU_FRAME_SKIPPING))
extern S32 wgui_fixed_list_menu_frame_counter;
#endif 


/*****************************************************************************
 * FUNCTION
 *  reset_dynamic_list
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void reset_dynamic_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_stop_list_highlight_effect();
#endif 
     

#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_LIST_MENU_FRAME_SKIPPING))
    gui_cancel_timer(wgui_dynamic_list_menu_end_frame);
    wgui_fixed_list_menu_frame_counter = 0;
#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_LIST_MENU_FRAME_SKIPPING)) */ 
    normal_fixed_list();
     
#if defined (__MMI_UI_TRANSPARENT_EFFECT__) || defined (__MMI_UI_LIST_HIGHLIGHT_EFFECTS__)       
    gui_reset_transparent_source_layer();
#endif 
     
    clear_keyboard_key_handler();
    ClearKeyHandler(KEY_UP_ARROW, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    MMI_fixed_list_menu.item_highlighted = dummy_highlight_handler;
    wgui_text_menuitem_reset_scrolling();
     
#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
    reset_fixed_icontext_menuitem_type();
#endif 
     
}


/*****************************************************************************
 * FUNCTION
 *  show_dynamic_list
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void show_dynamic_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_stop_list_highlight_effect();
#endif 
#if (defined __MMI_UI_TWO_LINE_MENUITEM_STYLES__ || defined __MMI_UI_HINTS_IN_MENUITEM__)
    wgui_two_line_stop_scroller();
#endif 
    if (MMI_fixed_list_menu.flags & UI_LIST_MENU_DISABLE_BACKGROUND)
    {
        _MMI_hide_fixed_list_menu();
    }
    /* gui_hide_animations(); */
    gui_show_dynamic_list_menu(&MMI_fixed_list_menu);
    wgui_display_dynamic_text_list_pop_up_description();
    gui_BLT_double_buffer(
        MMI_fixed_list_menu.x,
        MMI_fixed_list_menu.y,
        MMI_fixed_list_menu.x + MMI_fixed_list_menu.width,
        MMI_fixed_list_menu.y + MMI_fixed_list_menu.height);
}


/*****************************************************************************
 * FUNCTION
 *  redraw_dynamic_list
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void redraw_dynamic_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_current_menu_type == ANI_BG_LIST_MENU)
    {
        return;
    }
#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_LIST_MENU_FRAME_SKIPPING))
    if (wgui_fixed_list_menu_frame_counter > 0)
    {
        wgui_fixed_list_menu_frame_counter++;
        gui_show_dynamic_list_menu_no_draw(&MMI_fixed_list_menu);
    }
    else
    {
        wgui_fixed_list_menu_frame_counter = 1;
        gui_show_dynamic_list_menu_no_draw(&MMI_fixed_list_menu);
        gui_start_timer(UI_FRAME_START_TIMEOUT, wgui_dynamic_list_menu_end_frame);
    }
#else /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_LIST_MENU_FRAME_SKIPPING)) */ 
    show_dynamic_list();
#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_LIST_MENU_FRAME_SKIPPING)) */ 
}


/*****************************************************************************
 * FUNCTION
 *  dynamic_list_goto_previous_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dynamic_list_goto_previous_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_fixed_list_menu.n_items <= 0)
    {
        return;
    }

#if defined(__PROJECT_GALLITE_C01__) 
    if(MMI_fixed_list_menu.highlighted_item == 0)
    {
        if(IsInDialerSrc() && !IsFocusOnInputBox())
        {
            chang_dialer_focus();

            return;
        }
    }
#endif
    if ((MMI_fixed_list_menu.n_items == 1) && (MMI_fixed_list_menu.highlighted_item == 0))
    {
        return;
    }
     
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_stop_list_highlight_effect();
#endif 
     

    gui_dynamic_list_menu_goto_previous_item(&MMI_fixed_list_menu);

     
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
     
    //gui_stop_list_highlight_effect();
     
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
     
    redraw_dynamic_list();
}


/*****************************************************************************
 * FUNCTION
 *  dynamic_list_goto_next_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void dynamic_list_goto_next_item(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_fixed_list_menu.n_items <= 0)
    {
        return;
    }
    if ((MMI_fixed_list_menu.n_items == 1) && (MMI_fixed_list_menu.highlighted_item == 0))
    {
        return;
    }
     
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    gui_stop_list_highlight_effect();
#endif 
     

    gui_dynamic_list_menu_goto_next_item(&MMI_fixed_list_menu);

     
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
     
    //gui_stop_list_highlight_effect();
     
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
     
    redraw_dynamic_list();
}


/*****************************************************************************
 * FUNCTION
 *  dynamic_list_goto_item
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item_index      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void dynamic_list_goto_item(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_fixed_list_menu.n_items <= 0)
    {
        return;
    }
    if (item_index == MMI_fixed_list_menu.highlighted_item)
    {
        return;
    }
     
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
    /* To handle the redraw of hilighter if item index is greater then no of items */
    if (item_index >= MMI_fixed_list_menu.n_items)
    {
        return;
    }
     
    gui_stop_list_highlight_effect();
     
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
     
    gui_dynamic_list_menu_goto_item(&MMI_fixed_list_menu, item_index);

     
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
     
    //gui_stop_list_highlight_effect();
     
#endif /* __MMI_UI_LIST_HIGHLIGHT_EFFECTS__ */ 
     
    redraw_dynamic_list();
}


/*****************************************************************************
 * FUNCTION
 *  dynamic_list_goto_item_no_redraw
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item_index      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void dynamic_list_goto_item_no_redraw(S32 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_fixed_list_menu.n_items <= 0)
    {
        return;
    }
    if (item_index < 0)
    {
        item_index = 0;
    }
    if (item_index > (MMI_fixed_list_menu.n_items - 1))
    {
        item_index = MMI_fixed_list_menu.n_items - 1;
    }
    MMI_fixed_list_menu.highlighted_item = -1;
    gui_dynamic_list_menu_goto_item(&MMI_fixed_list_menu, item_index);
}


/*****************************************************************************
 * FUNCTION
 *  register_dynamic_list_shortcut_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_dynamic_list_shortcut_handler(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    register_menu_shortcut_keys();
    register_menu_shortcut_selected_only(dynamic_list_goto_item);
}


/*****************************************************************************
 * FUNCTION
 *  register_dynamic_list_keys
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void register_dynamic_list_keys(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    clear_keyboard_key_handler();
    SetKeyHandler(dynamic_list_goto_previous_item, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(dynamic_list_goto_next_item, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(dynamic_list_goto_previous_item, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(dynamic_list_goto_next_item, KEY_VOL_DOWN, KEY_EVENT_DOWN);
}

#if((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_LIST_MENU_FRAME_SKIPPING))


/*****************************************************************************
 * FUNCTION
 *  wgui_dynamic_list_menu_end_frame
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void wgui_dynamic_list_menu_end_frame(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (wgui_fixed_list_menu_frame_counter >= 1)
    {
        if (MMI_fixed_list_menu.flags & UI_LIST_MENU_DISABLE_BACKGROUND)
        {
            _MMI_hide_fixed_list_menu();
        }
        gui_hide_animations();
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gui_lock_double_buffer();
    #endif 
        gui_show_dynamic_list_menu(&MMI_fixed_list_menu);
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gui_unlock_double_buffer();
    #endif 
        wgui_display_dynamic_text_list_pop_up_description();    /* ????? */
    #if(UI_DOUBLE_BUFFER_SUPPORT)
        gui_BLT_double_buffer(
            MMI_fixed_list_menu.x,
            MMI_fixed_list_menu.y,
            MMI_fixed_list_menu.x + MMI_fixed_list_menu.width,
            MMI_fixed_list_menu.y + MMI_fixed_list_menu.height);
    #endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
        wgui_fixed_list_menu_frame_counter = 0;
        gui_start_timer(UI_FRAME_SKIP_TIMEOUT, wgui_dynamic_list_menu_end_frame);
    }
}

#endif /* ((UI_ENABLE_FRAME_SKIPPING) && (ENABLE_LIST_MENU_FRAME_SKIPPING)) */ 


/*****************************************************************************
 * FUNCTION
 *  DynamicListIncreaseItem
 * DESCRIPTION
 *  Increase the total number of dynamic item
 * PARAMETERS
 *  steps       [IN]        Number of items to be increased
 * RETURNS
 *  void
 *****************************************************************************/
void DynamicListIncreaseItem(S16 steps)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dynamic_item_buffer.n_total_items = dynamic_item_buffer.n_total_items + steps;
    MMI_fixed_list_menu.n_items = MMI_fixed_list_menu.n_items + steps;
}


/*****************************************************************************
 * FUNCTION
 *  DynamicListGetItems
 * DESCRIPTION
 *  Returns total number of dynamic items and the requested item from the dynamic buffer
 * PARAMETERS
 *  string          [OUT]       Requested item
 *  item_index      [IN]        Index of the requested item ,index is UI index
 * RETURNS
 *  total number of dynamic items
 *****************************************************************************/
S32 DynamicListGetItems(U8 *string, U16 item_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (string != NULL)
    {
        pfnUnicodeStrcpy((S8*) string, (S8*) MMI_fixed_icontext_menuitems[(item_index) % 32].item_text);
    }

    return MMI_fixed_list_menu.n_items;
}


/*****************************************************************************
 * FUNCTION
 *  DynamicListGetHighlightedItem
 * DESCRIPTION
 *  Returns the index of highlighted item, and highlighted item itself
 * PARAMETERS
 *  string      [OUT]       Highlighted item
 * RETURNS
 *  Highlighted item
 *****************************************************************************/
S32 DynamicListGetHighlightedItem(U8 *string)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (string != NULL)
    {
        pfnUnicodeStrcpy(
            (S8*) string,
            (S8*) MMI_fixed_icontext_menuitems[(MMI_fixed_list_menu.highlighted_item) % 32].item_text);
    }

    return MMI_fixed_list_menu.highlighted_item;

}


/*****************************************************************************
 * FUNCTION
 *  DynamicGetFirstDisplayedItem
 * DESCRIPTION
 *  Returns the index of first displayed item, and highlighted item itself
 * PARAMETERS
 *  string      [OUT]       First displayed item
 * RETURNS
 *  first displayed item item
 *****************************************************************************/
S32 DynamicGetFirstDisplayedItem(U8 *string)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (string != NULL)
    {
        pfnUnicodeStrcpy(
            (S8*) string,
            (S8*) MMI_fixed_icontext_menuitems[(MMI_fixed_list_menu.first_displayed_item) % 32].item_text);
    }

    return MMI_fixed_list_menu.first_displayed_item;

}
#ifdef __FT_IDLE_SMART_DIAL__
void register_dynamic_SmartDial_list_keys( void )
{
	clear_keyboard_key_handler();
	SetKeyHandler( dynamic_SmartDial_list_goto_previous_item, KEY_UP_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( dynamic_SmartDial_list_goto_next_item, KEY_DOWN_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( dynamic_SmartDial_list_goto_previous_item, KEY_VOL_UP, KEY_EVENT_DOWN );
	SetKeyHandler( dynamic_SmartDial_list_goto_next_item, KEY_VOL_DOWN, KEY_EVENT_DOWN );
}


/*****************************************************************************
* FUNCTION
*     dynamic_SmartDial_list_goto_previous_item
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     external_global
*****************************************************************************/
void dynamic_SmartDial_list_goto_previous_item( void )
{
      //U8* pBuffer;
     // U32 l;
	if( MMI_fixed_list_menu.n_items <= 0 )
		return;
	if( (MMI_fixed_list_menu.n_items == 1) && (MMI_fixed_list_menu.highlighted_item == 0) )
		return;
	 
	#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
		gui_stop_list_highlight_effect();
	#endif

	gui_dynamic_list_menu_goto_previous_item( &MMI_fixed_list_menu );

	 
	redraw_dynamic_list();
}

/*****************************************************************************
* FUNCTION
*     dynamic_SmartDial_list_goto_next_item
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     external_global
*****************************************************************************/
void dynamic_SmartDial_list_goto_next_item( void )
{
      //U8* pBuffer;
      //U32 l;
	
	if( MMI_fixed_list_menu.n_items <= 0 )
		return;
	if( (MMI_fixed_list_menu.n_items == 1) && (MMI_fixed_list_menu.highlighted_item == 0) )
		return;
	 
	#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
		gui_stop_list_highlight_effect();
	#endif
    

	gui_dynamic_list_menu_goto_next_item( &MMI_fixed_list_menu );
	
	redraw_dynamic_list();
}
#endif  //for smartdial


