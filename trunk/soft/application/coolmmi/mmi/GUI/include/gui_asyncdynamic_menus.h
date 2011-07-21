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
*  gui_asyncdynamic_menus.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   In order to use limited memory (size decided in compile-time) to load unlimited items (size decide in run-time) 
*   to a category screen, a modification in MMI for dynamic item loading is required as the following.
*
* Author:
* -------
 
*
*==============================================================================
*           HISTORY
 
*------------------------------------------------------------------------------
* $Revision:   1.6  $ 
* $Modtime:   May 16 2005 23:19:28  $
* $Log:   //CSDvs01/vmdata/Maui_sw/archives/mcu/plutommi/MMI/GUI/GUI_INC/gui_asyncdynamic_menus.h-arc  $
 * 
 *    Rev 1.6   May 17 2005 01:56:02   BM_Trunk
 
 * add copyright and disclaimer statement
 * 
 *    Rev 1.5   Mar 09 2005 22:46:44   CSD00658
 * Add touch screen support.
 * Resolution for 10151: [MMI][GUI][Add Feature] Add touch screen support for other UI components
 * 
 *    Rev 1.4   Feb 25 2005 17:06:28   CSD00658
 * Add file header.
 * Resolution for 8475: [GUI][Revise] Revise code of basic UI components
*
*******************************************************************************/

#ifndef __GUI_ASYNCDYNAMIC_MENUS_H__
#define __GUI_ASYNCDYNAMIC_MENUS_H__

#include "gui_fixed_menus.h"
extern S32 gui_asyncdynamic_list_error;
extern void gui_show_asyncdynamic_list_menu(fixed_list_menu *m);
extern void gui_show_asyncdynamic_list_menu_no_draw(fixed_list_menu *m);

#ifdef __MMI_TOUCH_SCREEN__
extern BOOL gui_asyncdynamic_list_menu_translate_pen_event(
                fixed_list_menu *m,
                mmi_pen_event_type_enum pen_event,
                S16 x,
                S16 y,
                gui_list_pen_enum *menu_event);
#endif /* __MMI_TOUCH_SCREEN__ */ 

extern void switch_asynchighlighted_item(fixed_list_menu *m, S32 last_highlighted_item);

extern void gui_asyncdynamic_list_menu_goto_item(fixed_list_menu *m, S32 i);
extern void gui_asyncdynamic_list_menu_goto_next_item(fixed_list_menu *m);
extern void gui_asyncdynamic_list_menu_goto_previous_item(fixed_list_menu *m);

extern void gui_asyncdynamic_list_menu_locate_highlighted_item(fixed_list_menu *m);
extern void gui_asyncdynamic_list_menu_locate_previous_item(fixed_list_menu *m);
extern void gui_asyncdynamic_list_menu_locate_next_item(fixed_list_menu *m);

#endif /* __GUI_ASYNCDYNAMIC_MENUS_H__ */ 

