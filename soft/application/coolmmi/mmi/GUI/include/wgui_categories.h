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
 *  wgui_categories.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Ordinary categories.
 *  Author:
 * -------
 
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
   Filename:      wgui_categories.h
 
   Date Created:  August-07-2002
   Contains:      UI wrapper routines Header
               High Level Category screen wrappers Header
**********************************************************************************/

#ifndef __WGUI_CATEGORIES_H__
#define __WGUI_CATEGORIES_H__

#include "mmi_features.h"
#include "wgui.h"
#include "wgui_softkeys.h"
#include "wgui_menu_shortcuts.h"
#include "wgui_fixed_menus.h"
#include "wgui_fixed_menuitems.h"
#include "wgui_inputs.h"
#include "wgui_datetime.h"
#include "wgui_inline_edit.h"
#include "custmenures.h"
#include "gui_config.h"

 
#include "wgui_dynamic_menus.h"
#include "wgui_dynamic_menuitems.h"
 
#include "wgui_asyncdynamic_menus.h"
#include "wgui_asyncdynamic_menuitems.h"

#include "wgui_categories_util.h"
/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-10 16:54 */
#include "wgui_categories_inputs.h"
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-10 16:54 */
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

#define MAX_CATEGORY89_STRINGS            64
#define MAX_CATEGORY89_STRING_LENGTH      8

#define CATEGORY_RES_TYPE_IMAGE           0
#define CATEGORY_RES_TYPE_VIDEO           1

typedef enum MMI_IDLE_BG_MEDIA_ENUM
{
    MMI_IDLE_BG_MEDIA_START,
    MMI_IDLE_BG_MEDIA_IMG,
    MMI_IDLE_BG_MEDIA_SWFLASH,
    MMI_IDLE_BG_MEDIA_END
} mmi_idle_bg_media_enum;
    

    extern void show_vertical_bar_previous(void);
    extern void show_vertical_bar_next(void);
    extern void show_vertical_bar(S32 volume_level);

/********** CATEGORY SCREEN START   **********/
    extern void ShowCategoryDummyScreen(void);//061306 dummy category
    extern void ExitCategoryDummyScreen(void);
    extern void ShowCategory1Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U16 *list_of_items,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ShowCategory2Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U16 message,
                    U8 *history_buffer);
    extern void ShowCategory6Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    U8 **list_of_descriptions,
                    S32 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory501Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U16 message,
                    U8 *history_buffer);

    extern void ShowCategory7Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *message,
                    U8 *history_buffer);
    extern void ExitCategory7Screen(void);

    extern void ShowCategory8Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U16 message,
                    U16 message_icon,
                    U8 *history_buffer);
    extern void ShowCategory208Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S8 *message,
                    U16 message_icon,
                    U8 *history_buffer);

    extern void ShowCategory9Screen(U16 message, U16 message_icon, U8 *history_buffer);
    extern void ExitCategory9Screen(void);

    extern void ShowCategory84Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    U16 *list_of_icons,
                    S32 flags,
                    S32 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory11Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U16 *list_of_items,
                    U16 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory12Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    U8 *list_of_states,
                    S32 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory13Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U16 *list_of_items,
                    U8 *list_of_states,
                    S32 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory15Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U16 *list_of_items,
                    U16 *list_of_icons,
                    S32 flags,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory15Screen(void);
    extern S32 GetCategory15HistorySize(void);
    extern U8 *GetCategory15History(U8 *history_buffer);

 
#ifdef UI_SMALL_LIST_SCREEN
    extern void ShowCategory10Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U16 *list_of_items,
                    U16 *list_of_icons,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory10Screen(void);
#endif /* UI_SMALL_LIST_SCREEN */ 
 

    extern void ShowCategory21Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U16 message,
                    U8 *history_buffer);

    extern void ShowCategory22Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 num_of_symbols,
                    U8 **MMI_symbols,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory22Screen(void);
    extern S32 GetCategory22HistorySize(void);
    extern U8 *GetCategory22History(U8 *history_buffer);

    extern void ShowCategory32Screen(
                    U8 *title,
                    PU8 title_icon,
                    U8 *left_softkey,
                    PU8 left_softkey_icon,
                    U8 *right_softkey,
                    PU8 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    PU8 *list_of_icons,
                    S32 flags,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory32Screen(void);
    extern U8 *GetCategory32History(U8 *history_buffer);

    extern void ShowCategory36Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    U16 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory101Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U16 parent_menu_item_ID,
                    U8 *history_buffer);

    extern void Category52ChangeItemText(S32 index, U8 *text);
    extern void Category52ChangeItemIcon(S32 index, U16 image_ID);
    extern void Category52ChangeItemDescription(S32 index, U8 *text);
    extern void Category52ChangeItemDescriptionNoAnimation(S32 index, U8 *text);

    extern void ShowCategory52Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U16 *list_of_items,
                    U16 *list_of_icons,
                    U8 **list_of_descriptions,
                    S32 flags,
                    S32 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory53Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    U16 *list_of_icons,
                    U8 **list_of_descriptions,
                    S32 flags,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory53Screen(void);
    extern U8 *GetCategory53History(U8 *history_buffer);
    /* Dynamic string */
    extern void ShowCategory353Screen(
                    U8 *title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    U16 *list_of_icons,
                    U8 **list_of_descriptions,
                    S32 flags,
                    S32 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory55Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U16 *list_of_items,
                    U8 **list_of_descriptions,
                    S32 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory56Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    U8 **list_of_descriptions,
                    S32 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory59Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *message,
                    U16 message_icon,
                    U8 *history_buffer);
    extern void ExitCategory59Screen(void);
    extern U8 *GetCategory59History(U8 *history_buffer);
    extern S32 GetCategory59HistorySize(void);

    extern void cat66_update_progress_string(void);

    extern void ShowCategory66Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *message,
                    U16 message_icon,
                    U8 *history_buffer);
    extern void ExitCategory66Screen(void);
    extern U8 *GetCategory66History(U8 *history_buffer);
    extern S32 GetCategory66HistorySize(void);

    extern void ShowCategory61Screen(U8 *message, U16 message_icon, U8 *history_buffer);
    extern void ExitCategory61Screen(void);
    extern U8 *GetCategory61History(U8 *history_buffer);
    extern S32 GetCategory61HistorySize(void);

    extern void RedrawCategory69Screen(void);
    extern void ShowCategory69Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U16 message,
                    S16 input_type,
                    U8 *buffer,
                    S32 buffer_size,
                    U8 *history_buffer);
    extern void ExitCategory69Screen(void);
    extern U8 *GetCategory69History(U8 *history_buffer);
    extern S32 GetCategory69HistorySize(void);
    void SetCategory69RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);

    extern void RedrawCategory72Screen(void);
    extern void ShowCategory72Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *message,
                    S32 percentage_value,
                    U8 *history_buffer);
    extern void ExitCategory72Screen(void);
    extern U8 *GetCategory72History(U8 *history_buffer);
    extern S32 GetCategory72HistorySize(void);

    extern void ShowCategory73Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    U16 *list_of_icons1,
                    U16 *list_of_icons2,
                    S32 highlighted_item,
                    U8 *history_buffer,
                    U8 u8display_flag);

    extern void RedrawCategory74Screen(void);
    extern void ShowCategory74Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *buffer,
                    S32 buffer_size,
                    U8 *history_buffer);
    extern void ExitCategory74Screen(void);
    extern U8 *GetCategory74History(U8 *history_buffer);
    extern S32 GetCategory74HistorySize(void);
    extern void ChangeCategory74MessageString(U8 *new_string);

#if defined(__UI_PAGING_IN_MULTILINE_EDITOR__)

    extern void ShowCategory75Screen(
                    U16 *name,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *buffer,
                    S32 buffer_size,
                    U8 *history_buffer,
                    gui_page_info_app **);
    extern void ExitCategory75Screen(void);

    extern void ShowCategory77Screen(
                    U16 *name,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *buffer,
                    S32 buffer_size,
                    U8 *history_buffer,
                    gui_page_info_app **);
    extern void ExitCategory77Screen(void);
#ifdef __MMI_TOUCH_SCREEN__
    extern void RedrawCategoryControlledArea75Screen(dm_coordinates *coordinate);
    extern void wgui_send_id_and_offset_to_app(U32 page_id, S16 page_offset);
#endif /* __MMI_TOUCH_SCREEN__ */ 
#endif /* defined(__UI_PAGING_IN_MULTILINE_EDITOR__) */ 

    extern void RedrawCategory76Screen(void);
    extern void ShowCategory76Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *buffer,
                    S32 buffer_size,
                    U8 *history_buffer);
    extern void ExitCategory76Screen(void);
    extern U8 *GetCategory76History(U8 *history_buffer);
    extern S32 GetCategory76HistorySize(void);
    extern void ChangeCategory76MessageString(U8 *new_string);

    extern void RedrawCategory78Screen(void);
    extern void ChangeCategory78MessageString(U8 *new_string);
    extern void ShowCategory78Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    multi_line_input_box_icon_hdlr icon_callback,
                    U8 *buffer,
                    S32 buffer_size,
                    U8 *history_buffer);
    extern void ExitCategory78Screen(void);
    extern S32 GetCategory78HistorySize(void);
    extern U8 *GetCategory78History(U8 *history_buffer);

    extern void ShowCategory87Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 l_limit,
                    S32 u_limit,
                    S32 *current_value,
                    U8 *history_buffer);

    extern void RedrawCategory88Screen(void);
    void ShowCategory88Screen(U16 title, U16 title_icon, U16 left_softkey, U16 left_softkey_icon, U16 right_softkey,
                              U16 right_softkey_icon, U16 bg_image_id, U16 n_items, U8 **string_list,
                              S32 *lower_limit, S32 *upper_limit, S32 **current_value, void (**cb_func) (S32 value),
                              U8 *history_buffer);
    extern void ExitCategory88Screen(void);
    void RegisterCategory88ChangeCallbacks(void (*f1) (S32 value), void (*f2) (S32 value));

    extern void RedrawCategory89Screen(void);
     
    extern void ShowCategory89Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    U8 **list_of_descriptions,
                    S32 flags,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory89Screen(void);
    extern U8 *GetCategory89History(U8 *history_buffer);

    extern void RedrawCategory90Screen(void);
    extern void ShowCategory90Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *message,
                    U16 caption1,
                    U16 caption2,
                    U8 *value1,
                    U8 *value2,
                    U8 *history_buffer);
    extern void ExitCategory90Screen(void);
    extern S32 GetCategory90HistorySize(void);
    extern U8 *GetCategory90History(U8 *history_buffer);

    extern void ShowCategory105Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 *volume_level,
                    U8 *history_buffer);
    extern S32 GetCategory105HistorySize(void);
    extern void ExitCategory105Screen(void);
    extern void RedrawCategory105Screen(void);
    extern U8 *GetCategory105History(U8 *history_buffer);

    extern void ShowCategory109Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    U8 **list_of_descriptions,
                    S32 highlighted_item,
                    U8 *history_buffer);

    void SetCategory111RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);
    extern void ShowCategory111Screen(
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U16 message,
                    S16 input_type,
                    U8 *buffer,
                    S32 buffer_size,
                    U8 *message2,
                    U8 *history_buffer);
    extern void ShowCategory111Screen_ext(U16 left_softkey, U16 left_softkey_icon, U16 right_softkey, U16 right_softkey_icon, U8 *message, S16 input_type, U8 *buffer, S32 buffer_size, U8 *message2, S32 duration, U8 *history_buffer);     
    extern void SetCat111TimerExpireFunc(FuncPtr f);
    extern void RedrawCategory111Screen(void);
    extern void ExitCategory111Screen(void);
    extern U8 *GetCategory111History(U8 *history_buffer);
    extern S32 GetCategory111HistorySize(void);

 
#ifdef UI_SMALL_PIN_EDITOR_SCREEN
    extern void ShowCategory112Screen(
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *message,
                    S16 input_type,
                    U8 *buffer,
                    S32 buffer_size,
                    U8 *message2,
                    U8 *history_buffer);
    extern void ExitCategory112Screen(void);
#endif /* UI_SMALL_PIN_EDITOR_SCREEN */ 
 

    extern void ShowCategory119Screen(
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *message,
                    U16 message_icon,
                    U8 *history_buffer);
    extern void RedrawCategory119Screen(void);
    extern void ExitCategory119Screen(void);
    extern U8 *GetCategory119History(U8 *history_buffer);
    extern S32 GetCategory119HistorySize(void);

/** SLide 31 **/
    extern void ShowCategory120Screen(
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U16 message,
                    U16 message_icon,
                    U8 *history_buffer);
    extern U8 *GetCategory120History(U8 *history_buffer);
    extern S32 GetCategory120HistorySize(void);
    extern void ExitCategory120Screen(void);
    extern void RedrawCategory120Screen(void);

    extern void RedrawCategory122Screen(void);
    extern void ShowCategory122Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U16 message,
                    S32 percentage_value,
                    U8 *history_buffer,
                    S8 *disp_val,
                    U8 *bar_status,
                    U16 side_icon,
                    U8 *,
                    U8 *);
    extern void ExitCategory122Screen(void);
    extern S32 GetCategory122HistorySize(void);
    extern U8 *GetCategory122History(U8 *history_buffer);

    extern void RedrawCategory125Screen(void);
    extern void ExitCategory125Screen(void);
    extern U8 *GetCategory125History(U8 *history_buffer);
    extern void ShowCategory125Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items1,
                    U8 **list_of_items2,
                    U16 *list_of_icons1,
                    S32 highlighted_item,
                    U8 *history_buffer,
                    U8 *top_msg,
                    U8 col_no);

    extern gdi_handle cat129_get_image_handle(void);//070706
    extern void ShowCategory129Screen(
                    U8 *title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U16 message_icon,
                    U8 *history_buffer);
    extern void ExitCategory129Screen(void);
    extern void RedrawCategory129Screen(void);

    extern void ShowCategory140Screen(
                    U16 title,
                    U16 title_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    U8 *list_of_states,
                    S32 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory141Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *message,
                    U16 message_icon,
                    U8 *history_buffer);
    extern void ExitCategory141Screen(void);
    extern void RedrawCategory141Screen(void);

    extern void ShowCategory142Screen(
                    U8 *title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 **list_of_titles,
                    U16 *list_of_images,
                    U8 **file_list_of_images,
                    S32 n_items,
                    S32 *index,
                    U8 *history_buffer);
    extern void ExitCategory142Screen(void);
    extern U8 *GetCategory142History(U8 *history_buffer);
    extern S32 GetCategory142HistorySize(void);
    extern void RedrawCategory142Screen(void);

    extern void ShowCategory150Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items1,
                    U8 **list_of_items2,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory150Screen(void);
    extern void RedrawCategory150Screen(void);

    extern void RedrawCategory152Screen(void);
    extern void ShowCategory152Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *message,
                    U8 *history_buffer);
    extern void ExitCategory152Screen(void);
    extern S32 GetCategory152HistorySize(void);
    extern U8 *GetCategory152History(U8 *history_buffer);

    extern void ShowCategory153Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items1,
                    U8 **list_of_items2,
                    S32 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory154Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *message1,
                    U8 *message2,
                    U16 message_icon,
                    U8 *history_buffer);
    extern void ExitCategory154Screen(void);
    extern void RedrawCategory154Screen(void);
    extern void ChangeCategory154Data(U8 *message1, U8 *message2, U16 message_icon);
    extern void Setcategory154LeftAlign(void);
#if defined(__MMI_SMART_MESSAGE_MT__) || ( defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__))
#endif /* defined(__MMI_SMART_MESSAGE_MT__) || ( defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) */ 

    void ShowCategory166Screen(U16 image_id, S8 *image_filename, void (*animation_complete_callback) (S32 result));
    extern void RedrawCategory166Screen(void);
    extern void ExitCategory166Screen(void);
    extern void StopCategory166Animation(void);

    extern void ShowCategory170Screen(
                    U8 *title,
                    PU8 title_icon,
                    U8 *left_softkey,
                    PU8 left_softkey_icon,
                    U8 *right_softkey,
                    PU8 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    PU8 *list_of_icons,
                    S32 flags,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory170Screen(void);
    extern void RedrawCategory170Screen(void);

    extern void ShowCategory172Screen(
                    U8 *title,
                    PU8 title_icon,
                    U8 *left_softkey,
                    PU8 left_softkey_icon,
                    U8 *right_softkey,
                    PU8 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    S32 flags,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory172Screen(void);
    extern void RedrawCategory172Screen(void);

    extern void ShowCategory174Screen(
                    U8 *title,
                    PU8 title_icon,
                    U8 *left_softkey,
                    PU8 left_softkey_icon,
                    U8 *right_softkey,
                    PU8 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    PU8 *list_of_icons,
                    U8 **list_of_descriptions,
                    S32 flags,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory174Screen(void);
    extern void RedrawCategory174Screen(void);
    /* micha1119 */
    extern void ShowCategory175Screen(
                    PU8 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *message,
                    U8 *radio_option1,
                    U8 *radio_option2,
                    U16 highlighted_item,
                    U8 *history_buffer);

    extern void Category174ChangeItemText(S32 index, U8 *text);
    extern void Category174ChangeItemIcon(S32 index, U16 image_ID);
    extern void Category174ChangeItemDescription(S32 index, U8 *text);

    extern void ShowCategory176Screen(
                    U8 *title,
                    PU8 title_icon,
                    U8 *left_softkey,
                    PU8 left_softkey_icon,
                    U8 *right_softkey,
                    PU8 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items,
                    PU8 *list_of_icons,
                    S32 flags,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory176Screen(void);
    extern void RedrawCategory176sScreen(void);

    extern void RedrawCategory184Screen(void);
    extern void ShowCategory184Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    GetItemFuncPtr get_item_func,
                    GetHintFuncPtr get_hint_func,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory184Screen(void);
    extern S32 GetCategory184HistorySize(void);
    extern U8 *GetCategory184History(U8 *history_buffer);

    void ShowCategory185Screen(U16 title, U16 title_icon, U16 left_softkey, U16 left_softkey_icon, U16 right_softkey,
                               U16 right_softkey_icon, S32 number_of_items, GetItemFuncPtr get_item_func,
                               GetHintFuncPtr get_hint_func, S32 highlighted_item, MMI_BOOL disable_data_cache,
                               void (*flush_data_callback) (void), U8 *history_buffer);

    extern void Category186ToggleCheckBox(void);
    void ShowCategory186Screen(U16 title, U16 title_icon, S32 number_of_items,
                               pBOOL(*get_text_callback) (S32 item_index, UI_string_type str_buff),
                               GetHintFuncPtr get_hint_func, U8 *list_of_states,
                               S32 highlighted_item, U8 *history_buffer);
    extern void ExitCategory186Screen(void);
    extern S32 GetCategory186HistorySize(void);
    extern U8 *GetCategory186History(U8 *history_buffer);

    extern void ShowCategory187Screen(
                    U16 title,
                    U16 title_icon,
                    S32 number_of_items,
                    pBOOL(*get_text_callback) (S32 item_index, UI_string_type str_buff),
                    GetHintFuncPtr get_hint_func,
                    U8 *list_of_states,S32 highlighted_item,
                    FuncPtr lsk_handler_func,
                    U8 *history_buffer);

    extern void ShowCategory284Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    GetItemFuncPtr get_item_func,
                    GetHintFuncPtr get_hint_func,
                    S32 highlighted_item,
                    U8 *history_buffer);

    extern void ShowCategory200Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    GetItemFuncPtr get_item_func,
                    GetHintFuncPtr get_hint_func,
                    S32 highlighted_item,
                    U16 search_image,
                    U8 *buffer,
                    S32 buffer_max_length,
                    U8 *history_buffer);
    extern void ExitCategory200Screen(void);
    extern U8 *GetCategory200History(U8 *history_buffer);
    extern S32 GetCategory200HistorySize(void);
    extern void RedrawCategory200Screen(void);
    extern void Cat200multitap_input_complete_callback(void);
    extern void Cat200multitap_inputcallback(UI_character_type inp);
    void RegisterCat200SearchFunction(S32(*search_function) (U8 *));
     
    void SetCategory200LeftSoftkeyFunction(void (*f) (void), MMI_key_event_type k);
    /* CSD end */
    void SetCategory200RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);
    extern void ShowCategory201Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    GetItemFuncPtr get_item_func,
                    GetHintFuncPtr get_hint_func,
                    S32 highlighted_item,
                    U16 search_image,
                    U8 *buffer,
                    S32 buffer_max_length,
                    S16 input_type,
                    U8 *history_buffer,
                    S16 *required_input_mode_set);
    void SetCategory201RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);
    extern void cat201_virtual_keypad_callback(void);
    void cat201_register_language_changed_indicator(BOOL(*pfunc) (U8));
    void Register_appl_present_function(BOOL(*application_present_function) (S32 *));
/** SMS chat screen **/

     
    extern void wgui_handle_inputbox_change_input_mode_bymultitapthai(void);

#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
    extern void ShowCategory273Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    GetItemFuncPtr get_item_func,
                    wgui_get_display_style get_display_style_func,
                    wgui_get_two_line_menu_item_properties get_properties_func,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void RedrawCategory273Screen(void);
    extern U8 *GetCategory273History(U8 *history_buffer);
    extern void ExitCategory273Screen(void);
#endif /* __MMI_UI_TWO_LINE_MENUITEM_STYLES__ */ 
//barcode
    extern void ShowCategory228Screen(
                    U16 title,U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8* buffer,
                    S32 buffer_size,
                    HILITE_STR hilite_str_list[],
                    U16 hilite_str_number,
                    void (*hilite_cb)(S32 idx),
                    U8* history_buffer);
    extern void RedrawCategory301Screen(void);
    extern void ShowCategory301Screen(U8 *message, U16 message_icon, U8 *history_buffer);
    extern void ExitCategory301Screen(void);
    extern S32 GetCategory301HistorySize(void);
    extern U8 *GetCategory301History(U8 *history_buffer);

    extern void RedrawCategory302Screen(void);
    extern void ShowCategory302Screen(U8 *history_buffer);
    extern void ExitCategory302Screen(void);
    extern S32 GetCategory302HistorySize(void);
    extern U8 *GetCategory302History(U8 *history_buffer);
    extern void ShowCategory303Screen(
                    U8 *message1,
                    U8 *message2,
                    FLOAT time_diff1,
                    FLOAT time_diff2,
                    U8 *history_buffer);
    extern void RedrawCategory303Screen(void);
    /* void ShowCategory303Screen(U8* message1,U8* message2,MYTIME *diff1,MYTIME *diff2,U8* history_buffer); */
    extern void ExitCategory303Screen(void);
    extern S32 GetCategory303HistorySize(void);
    extern U8 *GetCategory303History(U8 *history_buffer);

    extern void RedrawCategory304Screen(void);
    extern void ShowCategory304Screen(U16 message_icon, U8 *history_buffer);
    extern void ExitCategory304Screen(void);
    extern S32 GetCategory304HistorySize(void);
    extern U8 *GetCategory304History(U8 *history_buffer);

    /* CSD: Brian added, for engineer mode */
#ifdef __MMI_ENGINEER_MODE__     
    extern void RedrawCategory204Screen(void);
    extern void ShowCategory204Screen(
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U16 NotificationStringId,
                    U16 value,
                    U16 imageId,
                    U8 *history_buffer);
    extern void ExitCategory204Screen(void);
    /* CSD: Brian end */
#endif /* __MMI_ENGINEER_MODE__ */ 

    extern void RedrawCategory311Screen(void);
    extern void ShowCategory311Screen(U8 *message, U16 message_icon, U8 *history_buffer);
    extern void ExitCategory311Screen(void);
    extern S32 GetCategory311HistorySize(void);
    extern U8 *GetCategory311History(U8 *history_buffer);

    extern void RedrawCategory312Screen(void);
    /* void ShowCategory312Screen(U8* message,MYTIME *diff,U8* history_buffer); */
    extern void ExitCategory312Screen(void);
    extern S32 GetCategory312HistorySize(void);
    extern U8 *GetCategory312History(U8 *history_buffer);
    extern void ShowCategory312Screen(U8 *message, FLOAT time_diff_312, U8 *history_buffer);
    extern void ShowStatusIconsTitle(void);
    extern void ShowCategory313Screen(U16 message_icon, U8 *history_buffer);
    extern void RedrawCategory313Screen(void);
    extern void ExitCategory313Screen(void);
    extern U8 *GetCategory313History(U8 *history_buffer);
    extern S32 GetCategory313HistorySize(void);

    extern void ShowCategoryEMSScreen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 num_of_symbols,
                    UI_image_ID_type  *symbols,
                    UI_image_ID_type  *descriptions,
                    S32 highlighted_item,
                    U8 *history_buffer);

    extern U8 *GetCategory400History(U8 *history_buffer);
    extern S32 GetCategory400HistorySize(void);
    extern void ShowCategory400Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *history_buffer);
    extern void RedrawCategory400Screen(void);
    extern void ExitCategory400Screen(void);

#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_SMART_MESSAGE_MT__) || defined(__MMI_MYPICTURE__)
    extern void mmi_nsm2_category275_draw_tile(U32 strID);
    extern void mmi_nsm2_category275_draw_header(U32 identifier, S32 y_offset, S32 x1, S32 y1, S32 x2, S32 y2);
    extern void mmi_nsm2_category275_draw_inbox_header(S32 yy, S32 x1, S32 y1, S32 x2, S32 y2);
#endif /* (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_SMART_MESSAGE_MT__) || defined(__MMI_MYPICTURE__) */ 

    extern void ShowCategory354Screen(
                    U8 *title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    S32 number_of_items,
                    U8 **list_of_items1,
                    U8 **list_of_items2,
                    U16 *list_of_icons,
                    S32 highlighted_item,
                    U8 *history_buffer);
    extern void ExitCategory354Screen(void);

    extern void wgui_audio_equalizer_activate_bar(U8 bar_index);
    extern void wgui_audio_equalizer_change_sound_level(void);
    extern void ShowCategory334Screen(
                    U16 title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    signed char *values,
                    U8 *history_buffer);

    extern void ShowCategory402Screen(
                    U8 *title,
                    U16 title_icon,
                    U16 left_softkey,
                    U16 left_softkey_icon,
                    U16 right_softkey,
                    U16 right_softkey_icon,
                    U8 *string,
                    U16 percentage,
                    U8 *progress_string);
    extern void UpdateCategory402Value(U16 value, U8 *string);

#ifdef __cplusplus
};
#endif 
#ifdef __FT_IDLE_SMART_DIAL__ 
extern void RegisterCatSmartDialSearchFunction(S32 (*search_function)(U8 *));
extern void RedrawCategorySmartDialScreen(dm_coordinates* coordinate);
extern void ShowCategorySmartDialScreen(U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,S32 number_of_items,
	                                                                    GetItemFuncPtr get_item_func, GetHintFuncPtr	get_hint_func,
	                                                                         S32 highlighted_item,	U16 search_image,	U8 * buffer ,	U32 BufferLength,U8* history_buffer );
extern void ExitCategorySmartDialScreen(void);
extern void refresh_smartdial_search_list(U8 * number);
#endif

#endif /* __WGUI_CATEGORIES_H__ */ 

