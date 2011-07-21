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
*  wgui_dynamic_menus.h
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

#ifndef __WGUI_DYNAMIC_MENUS_H__
#define __WGUI_DYNAMIC_MENUS_H__

#include "wgui.h"

extern void show_dynamic_list(void);
extern void reset_dynamic_list(void);
extern void redraw_dynamic_list(void);

extern void wgui_dynamic_list_menu_end_frame(void);

extern void register_dynamic_list_shortcut_handler(void);
extern void register_dynamic_list_keys(void);

extern void dynamic_list_goto_item(S32 item_index);
extern void dynamic_list_goto_item_no_redraw(S32 item_index);
extern void dynamic_list_goto_previous_item(void);
extern void dynamic_list_goto_next_item(void);

 
#if defined (__MMI_UI_TRANSPARENT_EFFECT__) || defined (__MMI_UI_LIST_HIGHLIGHT_EFFECTS__)       
extern void gui_reset_transparent_source_layer(void);
#endif 
 
extern void DynamicListIncreaseItem(S16 steps);
extern S32 DynamicListGetItems(U8 *string, U16 item_index);
extern S32 DynamicListGetHighlightedItem(U8 *string);
extern S32 DynamicGetFirstDisplayedItem(U8 *string);
#ifdef __FT_IDLE_SMART_DIAL__
extern void register_dynamic_SmartDial_list_keys( void );
extern void dynamic_SmartDial_list_goto_previous_item( void );
extern void dynamic_SmartDial_list_goto_next_item( void );
#endif

#endif /* __WGUI_DYNAMIC_MENUS_H__ */ 

