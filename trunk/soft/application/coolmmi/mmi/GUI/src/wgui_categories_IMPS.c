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
 *  wgui_categories_IMPS.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  IMPS related categories. 
 *  [Category420]       About Me Screen
 *  [Category425]       Contact List Screen (also used by PoC)
 *  [Category435]       Chat Screen with Popup Editor
 *  TTY related categories.
 *  [Category430]       Chat Log Screen 
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
#include "wgui_tab_bars.h"
#include "wgui_categories.h"
#include "wgui_categories_defs.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_imps.h"
#include "wgui_categories_popup.h"
#include "wgui_inputs.h"
#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#include "editorpen.h"
#include "wgui_virtual_keyboard.h"
#endif /* __MMI_TOUCH_SCREEN__ */ 

#include "gdi_include.h"
#include "gd_primitive.h"
#include "kal_non_specific_general_types.h"     /* include this for kal data type */
#include "lcd_sw_rnd.h"
#include "gui_setting.h"

#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_SMART_MESSAGE_MT__)
#endif 

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
extern S32 gui_asyncdynamic_list_error;
extern BOOL r2lMMIFlag;

#ifdef __MMI_IMPS__
/***************************************************************************** 
* Customizable Definitions
*****************************************************************************/
/* 
 * Category 420 
 */
#if defined(__MMI_MAINLCD_128X160__)

/* Whether to display mood/status description text */
#undef CAT420_DISP_INFO_TEXT
#define CAT420_TOP_MARGIN                    2
/* Between information and description text */
#define CAT420_BOTTOM_MARGIN                 4
#define CAT420_LEFT_MARGIN                   5
#define CAT420_RIGHT_MARGIN                  3
#define CAT420_IMAGE_H_SPACING               (CAT420_RIGHT_MARGIN)
#define CAT420_LINE_EXTRA_PIXEL              10
#define CAT420_NAME_LINE_V_SPACING           3
#define CAT420_LINE_IMAGE_V_SPACING          6
#define CAT420_CAPTION_IMAGE_H_SPACING       6
#define CAT420_MOOD_STATUS_V_SPACING         3
/* Default image width */
#define CAT420_IMAGE_AREA_WIDTH              (60)
/* Minimum image width */
#define CAT420_IMAGE_AREA_MIN_WIDTH          (40)
#define CAT420_EDITOR_LEFT_BORDER_PADDING    (CAT420_LEFT_MARGIN)
#define CAT420_EDITOR_RIGHT_BORDER_PADDING   (CAT420_RIGHT_MARGIN)
#define CAT420_EDITOR_BOTTOM_PADDING         (5)

#elif defined(__MMI_MAINLCD_160X128__)

/* Whether to display mood/status description text */
#undef CAT420_DISP_INFO_TEXT
#define CAT420_TOP_MARGIN                    2
/* Between information and description text */
#define CAT420_BOTTOM_MARGIN                 4
#define CAT420_LEFT_MARGIN                   5
#define CAT420_RIGHT_MARGIN                  3
#define CAT420_IMAGE_H_SPACING               (CAT420_RIGHT_MARGIN)
#define CAT420_LINE_EXTRA_PIXEL              10
#define CAT420_NAME_LINE_V_SPACING           3
#define CAT420_LINE_IMAGE_V_SPACING          6
#define CAT420_CAPTION_IMAGE_H_SPACING       6
#define CAT420_MOOD_STATUS_V_SPACING         3
/* Default image width */
#define CAT420_IMAGE_AREA_WIDTH              (60)
/* Minimum image width */
#define CAT420_IMAGE_AREA_MIN_WIDTH          (40)
#define CAT420_EDITOR_LEFT_BORDER_PADDING    (CAT420_LEFT_MARGIN)
#define CAT420_EDITOR_RIGHT_BORDER_PADDING   (CAT420_RIGHT_MARGIN)
#define CAT420_EDITOR_BOTTOM_PADDING         (5)

#elif defined(__MMI_MAINLCD_176X220__)

/* Whether to display mood/status description text */
#define CAT420_DISP_INFO_TEXT
#define CAT420_TOP_MARGIN                    2
/* Between information and description text */
#define CAT420_BOTTOM_MARGIN                 7
#define CAT420_LEFT_MARGIN                   7
#define CAT420_RIGHT_MARGIN                  5
#define CAT420_IMAGE_H_SPACING               (CAT420_RIGHT_MARGIN)
#define CAT420_LINE_EXTRA_PIXEL              10
#define CAT420_NAME_LINE_V_SPACING           3
#define CAT420_LINE_IMAGE_V_SPACING          6
#define CAT420_CAPTION_IMAGE_H_SPACING       6
#define CAT420_MOOD_STATUS_V_SPACING         3
/* Default image width */
#define CAT420_IMAGE_AREA_WIDTH              (65)
/* Minimum image width */
#define CAT420_IMAGE_AREA_MIN_WIDTH          (45)
#define CAT420_EDITOR_LEFT_BORDER_PADDING    (CAT420_LEFT_MARGIN)
#define CAT420_EDITOR_RIGHT_BORDER_PADDING   (CAT420_RIGHT_MARGIN)
#define CAT420_EDITOR_BOTTOM_PADDING         (7)

#elif defined(__MMI_MAINLCD_220X176__)

/* Whether to display mood/status description text */
#define CAT420_DISP_INFO_TEXT
#define CAT420_TOP_MARGIN                    2
/* Between information and description text */
#define CAT420_BOTTOM_MARGIN                 7
#define CAT420_LEFT_MARGIN                   7
#define CAT420_RIGHT_MARGIN                  5
#define CAT420_IMAGE_H_SPACING               (CAT420_RIGHT_MARGIN)
#define CAT420_LINE_EXTRA_PIXEL              10
#define CAT420_NAME_LINE_V_SPACING           3
#define CAT420_LINE_IMAGE_V_SPACING          6
#define CAT420_CAPTION_IMAGE_H_SPACING       6
#define CAT420_MOOD_STATUS_V_SPACING         3
/* Default image width */
#define CAT420_IMAGE_AREA_WIDTH              (65)
/* Minimum image width */
#define CAT420_IMAGE_AREA_MIN_WIDTH          (45)
#define CAT420_EDITOR_LEFT_BORDER_PADDING    (CAT420_LEFT_MARGIN)
#define CAT420_EDITOR_RIGHT_BORDER_PADDING   (CAT420_RIGHT_MARGIN)
#define CAT420_EDITOR_BOTTOM_PADDING         (7)

#else 
/* For 240x320. 128x128 is also compiled but not linked */

/* Whether to display mood/status description text */
#define CAT420_DISP_INFO_TEXT
#define CAT420_TOP_MARGIN                    2
/* Between information and description text */
#define CAT420_BOTTOM_MARGIN                 7
#define CAT420_LEFT_MARGIN                   6
#define CAT420_RIGHT_MARGIN                  6
#define CAT420_IMAGE_H_SPACING               (CAT420_RIGHT_MARGIN)
#define CAT420_LINE_EXTRA_PIXEL              15
#define CAT420_NAME_LINE_V_SPACING           4
#define CAT420_LINE_IMAGE_V_SPACING          8
#define CAT420_CAPTION_IMAGE_H_SPACING       6
#define CAT420_MOOD_STATUS_V_SPACING         4
/* Default image width */
#define CAT420_IMAGE_AREA_WIDTH              (100)
/* Minimum image width */
#define CAT420_IMAGE_AREA_MIN_WIDTH          (60)
#define CAT420_EDITOR_LEFT_BORDER_PADDING    (CAT420_LEFT_MARGIN)
#define CAT420_EDITOR_RIGHT_BORDER_PADDING   (CAT420_RIGHT_MARGIN)
#define CAT420_EDITOR_BOTTOM_PADDING         (7)

#endif 

#define CAT420_TEXT_COLOR_R                  64
#define CAT420_TEXT_COLOR_G                  64
#define CAT420_TEXT_COLOR_B                  64

#define CAT420_LINE_COLOR_R                  144
#define CAT420_LINE_COLOR_G                  104
#define CAT420_LINE_COLOR_B                  48

/* 
 * Category 425
 */
#if defined(__MMI_MAINLCD_128X160__)||defined(__MMI_MAINLCD_128X128__)

#define CAT425_DEFAULT_ICON_WIDTH            26
#define CAT425_DEFAULT_ICON_HEIGHT           26
#define CAT425_DEFAULT_ICON2_WIDTH           10
#define CAT425_DEFAULT_ICON2_HEIGHT          10
#define CAT425_ICON_X                        2
#define CAT425_ICON_Y                        4
#define CAT425_ICON_TEXT_GAP                 4
#define CAT425_TEXT_SCROLLBAR_GAP            2
#define CAT425_TEXT1_Y                       0
#define CAT425_TEXT2_Y                       16
#define CAT425_TEXT_HEIGHT                   16
#define CAT425_MENUITEM_HEIGHT               33
#define CAT425_MENUITEM_HEIGHT_NO_TAB        39
#define CAT425_TEXT2_BRACKET_GAP             1

#elif defined(__MMI_MAINLCD_160X128__)

#define CAT425_DEFAULT_ICON_WIDTH            26
#define CAT425_DEFAULT_ICON_HEIGHT           26
#define CAT425_DEFAULT_ICON2_WIDTH           10
#define CAT425_DEFAULT_ICON2_HEIGHT          10
#define CAT425_ICON_X                        2
#define CAT425_ICON_Y                        4
#define CAT425_ICON_TEXT_GAP                 4
#define CAT425_TEXT_SCROLLBAR_GAP            2
#define CAT425_TEXT1_Y                       0
#define CAT425_TEXT2_Y                       16
#define CAT425_TEXT_HEIGHT                   16
#define CAT425_MENUITEM_HEIGHT               33
#define CAT425_MENUITEM_HEIGHT_NO_TAB        39
#define CAT425_TEXT2_BRACKET_GAP             1

#elif defined(__MMI_MAINLCD_176X220__)

#define CAT425_DEFAULT_ICON_WIDTH            32
#define CAT425_DEFAULT_ICON_HEIGHT           32
#define CAT425_DEFAULT_ICON2_WIDTH           10
#define CAT425_DEFAULT_ICON2_HEIGHT          10
#define CAT425_ICON_X                        2
#define CAT425_ICON_Y                        1
#define CAT425_ICON_TEXT_GAP                 4
#define CAT425_TEXT_SCROLLBAR_GAP            2
#define CAT425_TEXT1_Y                       0
#define CAT425_TEXT2_Y                       17
#define CAT425_TEXT_HEIGHT                   17
#define CAT425_MENUITEM_HEIGHT               35
#define CAT425_MENUITEM_HEIGHT_NO_TAB        39
#define CAT425_TEXT2_BRACKET_GAP             1
#elif defined(__MMI_MAINLCD_220X176__)

#define CAT425_DEFAULT_ICON_WIDTH            32
#define CAT425_DEFAULT_ICON_HEIGHT           32
#define CAT425_DEFAULT_ICON2_WIDTH           10
#define CAT425_DEFAULT_ICON2_HEIGHT          10
#define CAT425_ICON_X                        2
#define CAT425_ICON_Y                        1
#define CAT425_ICON_TEXT_GAP                 4
#define CAT425_TEXT_SCROLLBAR_GAP            2
#define CAT425_TEXT1_Y                       0
#define CAT425_TEXT2_Y                       17
#define CAT425_TEXT_HEIGHT                   17
#define CAT425_MENUITEM_HEIGHT               35
#define CAT425_MENUITEM_HEIGHT_NO_TAB        39
#define CAT425_TEXT2_BRACKET_GAP             1
#else 
/* For 240x320. 128x128 is also compiled but not linked */

#define CAT425_DEFAULT_ICON_WIDTH            40
#define CAT425_DEFAULT_ICON_HEIGHT           42
#define CAT425_DEFAULT_ICON2_WIDTH           10
#define CAT425_DEFAULT_ICON2_HEIGHT          10
#define CAT425_ICON_X                        2
#define CAT425_ICON_Y                        5
#define CAT425_ICON_TEXT_GAP                 6
#define CAT425_TEXT_SCROLLBAR_GAP            3
#define CAT425_TEXT1_Y                       0
#define CAT425_TEXT2_Y                       27
#define CAT425_TEXT_HEIGHT                   22
#define CAT425_MENUITEM_HEIGHT               55
#define CAT425_MENUITEM_HEIGHT_NO_TAB        60
#define CAT425_TEXT2_BRACKET_GAP             2

#endif 

#if defined(__MMI_MAINLCD_240X320__)

#define CAT425_ICON_BORDER_COLOR_R           187
#define CAT425_ICON_BORDER_COLOR_G           227
#define CAT425_ICON_BORDER_COLOR_B           254

#define CAT425_ICON_HIGHLIGHT_BORDER_COLOR_R          128
#define CAT425_ICON_HIGHLIGHT_BORDER_COLOR_G          0
#define CAT425_ICON_HIGHLIGHT_BORDER_COLOR_B          0

// #define CAT425_TEXT1_COLOR_R                                         230
// #define CAT425_TEXT1_COLOR_G                                         230
// #define CAT425_TEXT1_COLOR_B                                         230

// #define CAT425_TEXT2_COLOR_R                                         148
// #define CAT425_TEXT2_COLOR_G                                         188
// #define CAT425_TEXT2_COLOR_B                                         220

// #define CAT425_TEXT1_HIGHLIGHT_COLOR_R                               255
// #define CAT425_TEXT1_HIGHLIGHT_COLOR_G                               255
// #define CAT425_TEXT1_HIGHLIGHT_COLOR_B                               255

// #define CAT425_TEXT2_HIGHLIGHT_COLOR_R                               255
// #define CAT425_TEXT2_HIGHLIGHT_COLOR_G                               255
// #define CAT425_TEXT2_HIGHLIGHT_COLOR_B                               106

#define CAT425_TEXT2_BRACKET_COLOR_R         187
#define CAT425_TEXT2_BRACKET_COLOR_G         227
#define CAT425_TEXT2_BRACKET_COLOR_B         254

#define CAT425_LINE_COLOR_R                  192
#define CAT425_LINE_COLOR_G                  192
#define CAT425_LINE_COLOR_B                  192

#else /* defined(__MMI_MAINLCD_240X320__) */ 

#define CAT425_ICON_BORDER_COLOR_R           0
#define CAT425_ICON_BORDER_COLOR_G           56
#define CAT425_ICON_BORDER_COLOR_B           136

#define CAT425_ICON_HIGHLIGHT_BORDER_COLOR_R          128
#define CAT425_ICON_HIGHLIGHT_BORDER_COLOR_G          0
#define CAT425_ICON_HIGHLIGHT_BORDER_COLOR_B          0

#define CAT425_TEXT1_COLOR_R                 72
#define CAT425_TEXT1_COLOR_G                 72
#define CAT425_TEXT1_COLOR_B                 72

#define CAT425_TEXT2_COLOR_R                 48
#define CAT425_TEXT2_COLOR_G                 88
#define CAT425_TEXT2_COLOR_B                 120

#define CAT425_TEXT1_HIGHLIGHT_COLOR_R          255
#define CAT425_TEXT1_HIGHLIGHT_COLOR_G          255
#define CAT425_TEXT1_HIGHLIGHT_COLOR_B          255

#define CAT425_TEXT2_HIGHLIGHT_COLOR_R          255
#define CAT425_TEXT2_HIGHLIGHT_COLOR_G          255
#define CAT425_TEXT2_HIGHLIGHT_COLOR_B          106

#define CAT425_TEXT2_BRACKET_COLOR_R         56
#define CAT425_TEXT2_BRACKET_COLOR_G         112
#define CAT425_TEXT2_BRACKET_COLOR_B         200

#define CAT425_LINE_COLOR_R                  142
#define CAT425_LINE_COLOR_G                  142
#define CAT425_LINE_COLOR_B                  142

#endif /* defined(__MMI_MAINLCD_240X320__) */ 

/* 
 * Category 430
 */

#define CAT430_NAME1_COLOR_R                 107
#define CAT430_NAME1_COLOR_G                 93
#define CAT430_NAME1_COLOR_B                 247

#define CAT430_NAME2_COLOR_R                 231
#define CAT430_NAME2_COLOR_G                 77
#define CAT430_NAME2_COLOR_B                 198

#define CAT430_NAME_FONT_ENUM                MEDIUM_FONT
#define CAT430_NAME_FONT                     (&MMI_medium_font)

#define CAT430_SENT_COLOR_MARKER             GUI_INPUT_COLOR_BLACK

 
#define CAT430_MARKER_COUNT                  2

#ifdef __MMI_MAINLCD_240X320__
#define CAT430_VERTICAL_GAP                  4
#define CAT430_HORIZONTAL_GAP                3
#define CAT430_SENDING_BOX_V_GAP             2
#define CAT430_SENDING_BOX_H_GAP             0
#else /* __MMI_MAINLCD_240X320__ */ 
#define CAT430_VERTICAL_GAP                  2
#define CAT430_HORIZONTAL_GAP                2
#define CAT430_SENDING_BOX_V_GAP             2
#define CAT430_SENDING_BOX_H_GAP             0
#endif /* __MMI_MAINLCD_240X320__ */ 

#if !defined(__MMI_MAINLCD_128X128__)
/* Show name outside logbox */
#undef CAT430_NAME_IN_LOG
#else /* !defined(__MMI_MAINLCD_128X128__) */ 
/* Show name inside logbox */
#define CAT430_NAME_IN_LOG
#endif /* !defined(__MMI_MAINLCD_128X128__) */ 

/* 
 * Category 435
 */

#define CAT435_POPUP_X                       (MMI_content_x)
#define CAT435_POPUP_WIDTH                   (MMI_CONTENT_WIDTH_WITH_H_TAB)

#if	defined (__MMI_MAINLCD_128X160__) || defined (__MMI_MAINLCD_128X128__)

#define CAT435_POPUP_OVERLAP_TAB_BAR         1

#define CAT435_POPUP_Y                       (MMI_CONTENT_Y_WITH_H_TAB - 6)
#define CAT435_POPUP_HEIGHT                  (MMI_CONTENT_HEIGHT_WITH_H_TAB + 6)
#undef CAT435_SHOW_LOG_BOX_WITH_POPUP
#undef CAT435_POPUP_WITH_BORDER_TITLE
/* Title height in popup */
#define CAT435_POPUP_TITLE_HEIGHT            18
/* Bottom gap between editor and multitap */
#define CAT435_POPUP_BOTTOM_GAP              0  /* HACK. 0 for ZI input method */
/* Horizontal gap */
#define CAT435_POPUP_H_GAP                   0  /* HACK. 0 for ZI input method */

#elif defined(__MMI_MAINLCD_160X128__)

#define CAT435_POPUP_OVERLAP_TAB_BAR         1

#define CAT435_POPUP_Y                       (MMI_CONTENT_Y_WITH_H_TAB - 6)
#define CAT435_POPUP_HEIGHT                  (MMI_CONTENT_HEIGHT_WITH_H_TAB + 6)
#undef CAT435_SHOW_LOG_BOX_WITH_POPUP
#undef CAT435_POPUP_WITH_BORDER_TITLE
/* Title height in popup */
#define CAT435_POPUP_TITLE_HEIGHT            18
/* Bottom gap between editor and multitap */
#define CAT435_POPUP_BOTTOM_GAP              0  /* HACK. 0 for ZI input method */
/* Horizontal gap */
#define CAT435_POPUP_H_GAP                   0  /* HACK. 0 for ZI input method */

#elif defined(__MMI_MAINLCD_176X220__)

#undef CAT435_POPUP_OVERLAP_TAB_BAR

#ifdef __MMI_TOUCH_SCREEN__
/* Virtual keyboard takes more space */
#define CAT435_POPUP_Y                       (MMI_CONTENT_Y_WITH_H_TAB - 4)
#define CAT435_POPUP_HEIGHT                  (MMI_CONTENT_HEIGHT_WITH_H_TAB + 4)
#undef CAT435_SHOW_LOG_BOX_WITH_POPUP
#else /* __MMI_TOUCH_SCREEN__ */ 
#define CAT435_POPUP_Y                       (MMI_CONTENT_Y_WITH_H_TAB + (MMI_CONTENT_HEIGHT_WITH_H_TAB /5))
#define CAT435_POPUP_HEIGHT                  (MMI_CONTENT_HEIGHT_WITH_H_TAB - (MMI_CONTENT_HEIGHT_WITH_H_TAB /5))
#define CAT435_SHOW_LOG_BOX_WITH_POPUP
#endif /* __MMI_TOUCH_SCREEN__ */ 

#define CAT435_POPUP_WITH_BORDER_TITLE
/* Title height in popup */
#define CAT435_POPUP_TITLE_HEIGHT            19
/* Bottom gap between editor and multitap */
#define CAT435_POPUP_BOTTOM_GAP              0  /* HACK. 0 for ZI input method */
/* Horizontal gap */
#define CAT435_POPUP_H_GAP                   0  /* HACK. 0 for ZI input method */
#elif defined(__MMI_MAINLCD_220X176__)

#undef CAT435_POPUP_OVERLAP_TAB_BAR

#ifdef __MMI_TOUCH_SCREEN__
/* Virtual keyboard takes more space */
#define CAT435_POPUP_Y                       (MMI_CONTENT_Y_WITH_H_TAB - 4)
#define CAT435_POPUP_HEIGHT                  (MMI_CONTENT_HEIGHT_WITH_H_TAB + 4)
#undef CAT435_SHOW_LOG_BOX_WITH_POPUP
#else /* __MMI_TOUCH_SCREEN__ */ 
#define CAT435_POPUP_Y                       (MMI_CONTENT_Y_WITH_H_TAB + (MMI_CONTENT_HEIGHT_WITH_H_TAB /5))
#define CAT435_POPUP_HEIGHT                  (MMI_CONTENT_HEIGHT_WITH_H_TAB - (MMI_CONTENT_HEIGHT_WITH_H_TAB /5))
#define CAT435_SHOW_LOG_BOX_WITH_POPUP
#endif /* __MMI_TOUCH_SCREEN__ */ 

#define CAT435_POPUP_WITH_BORDER_TITLE
/* Title height in popup */
#define CAT435_POPUP_TITLE_HEIGHT            19
/* Bottom gap between editor and multitap */
#define CAT435_POPUP_BOTTOM_GAP              0  /* HACK. 0 for ZI input method */
/* Horizontal gap */
#define CAT435_POPUP_H_GAP                   0  /* HACK. 0 for ZI input method */

#else 
/* For 240x320. 128x128 is also compiled but not linked */

#ifdef __MMI_TOUCH_SCREEN__
/* Virtual keyboard takes more space */
#define CAT435_POPUP_Y                       (MMI_CONTENT_Y_WITH_H_TAB)
#define CAT435_POPUP_HEIGHT                  (MMI_CONTENT_HEIGHT_WITH_H_TAB)
#undef CAT435_SHOW_LOG_BOX_WITH_POPUP
#else /* __MMI_TOUCH_SCREEN__ */ 
#define CAT435_POPUP_Y                       (MMI_CONTENT_Y_WITH_H_TAB + (MMI_CONTENT_HEIGHT_WITH_H_TAB /5))
#define CAT435_POPUP_HEIGHT                  (MMI_CONTENT_HEIGHT_WITH_H_TAB - (MMI_CONTENT_HEIGHT_WITH_H_TAB /5))
#define CAT435_SHOW_LOG_BOX_WITH_POPUP
#endif /* __MMI_TOUCH_SCREEN__ */ 

#define CAT435_POPUP_WITH_BORDER_TITLE
/* Title height in popup */
#define CAT435_POPUP_TITLE_HEIGHT            30
/* Bottom gap between editor and multitap */
#define CAT435_POPUP_BOTTOM_GAP              0  /* HACK. 0 for ZI input method */
/* Horizontal gap */
#define CAT435_POPUP_H_GAP                   0  /* HACK. 0 for ZI input method */

#endif 

#define CAT435_BACKGROUND_WHITE_VALUE        (80)

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/* 
 * Category 420 
 */
typedef struct
{
    /* Data */
    UI_string_type name;
    U16 mood_caption;
    UI_string_type mood_string;
    PU8 mood_icon;
    U16 status_caption;
    UI_string_type status_string;
    PU8 status_icon;
    S16 image_ori_width, image_ori_height;
    /* Image cache */
    U32 display_image:1;
    U32 image_is_cached:1;
    gdi_handle cache_layer;
    /* Layout */
    S16 content_width;
    S16 editor_header_height;

    S16 name_x, name_y, name_width;
    S16 line_x1, line_x2, line_y;

    S16 mood_caption_x, mood_caption_y;
    S16 mood_image_x, mood_image_y;
#ifdef CAT420_DISP_INFO_TEXT
    S16 mood_text_x, mood_text_y;
#endif 

    S16 status_caption_x, status_caption_y;
    S16 status_image_x, status_image_y;
#ifdef CAT420_DISP_INFO_TEXT
    S16 status_text_x, status_text_y;
#endif 

    S16 image_area_x, image_area_y;
    S16 image_area_width, image_area_height;

    S16 image_x_offset, image_y_offset;
    S16 image_resized_width, image_resized_height;  /* 0 if no resize */

} gui_cat420_context_struct;

/* 
 * Category 425
 */
typedef struct
{
    U32 backup_menu_flags;
    UI_filled_area *backup_menu_filler;

    S16 icon_border_x1, icon_border_y1, icon_border_x2, icon_border_y2;
    S16 bracket_x, bracket_y;

    S16 num_tab;
    S32 content_x, content_y, content_width, content_height;

    /* For RefreshCategory425Screen */
    GetAsyncItemFuncPtr get_item_func;
    GetAsyncHintFuncPtr get_hint_func;
} gui_cat425_context_struct;

/* 
 * Category 430
 */
typedef struct
{
    /* Another input box */
    multi_line_input_box log_box1;
    U8 *allocated_color_buffer;          
    /* Data */
    U8 *name1, *name2;
    U8 *log_buffer1;
    U8 *log_buffer2;
    U8 *sending_buffer;
    S16 log_buffer_size1;
    S16 log_buffer_size2;
    S16 max_sending_buffer_size;
    S16 sent_count;
    S16 allocated_color_buffer_size;
    MMI_BOOL show_sending_buffer;
#ifdef __MMI_TOUCH_SCREEN__
    U8 pen_object;                      /* 0: none, 1: log_box1, 2: log_box2 */
#endif 
    /* Layout */
#ifndef CAT430_NAME_IN_LOG
    S16 name1_x, name1_y;
    S16 name2_x, name2_y;
#else /* CAT430_NAME_IN_LOG */ 
    S16 log_box_header_height;
#endif /* CAT430_NAME_IN_LOG */ 
    S16 log_box1_x, log_box1_y;
    S16 log_box1_width, log_box1_height;
    S16 log_box2_x, log_box2_y;
    S16 log_box2_width, log_box2_height;
    S16 log_box2_footer_height;
    S16 sending_box_x, sending_box_y;   /* relative coordinates */
    S16 sending_box_width;
} gui_cat430_context_struct;

typedef struct
{
    S32 dummy;
} gui_cat430_history_header_struct;

/* 
 * Category 435
 */
typedef struct
{
    /* Backup original MMI context */
    UI_filled_area *backup_multitap_filler;
    color_t backup_information_bar_color;
    S32 backup_MMI_editor_width, backup_MMI_editor_height;
    /* Log editor box */
    multi_line_input_box log_box;
    UI_filled_area log_box_scrollbar_sbutton_filler;
    /* Category parameters */
    U16 right_softkey;
    U16 right_softkey_icon;
    U16 left_softkey;
    U16 left_softkey_icon;
    U16 editor_right_softkey;
    U16 editor_right_softkey_icon;
    U16 editor_left_softkey;
    U16 editor_left_softkey_icon;
    UI_string_type popup_title;
    /* Softkey handlers */
    void (*editor_LSK_func) (void);
    void (*editor_RSK_func) (void);
    void (*LSK_func) (void);
    void (*RSK_func) (void);
    /* Whether the popup editor is displayed */
    BOOL show_popup;
    /* Data used to setup the popup editor */
    BOOL editor_created;    /* For debug only */
    S16 input_type;
    U8 *buffer;
    S8 icon_height;
    S32 buffer_size;
    multi_line_input_box_icon_hdlr icon_callback;
} gui_cat435_context_struct;

typedef struct
{
    U8 show_popup;
    U8 dummy[3];    /* Align to four bytes */
} gui_cat435_history_header_struct;

/***************************************************************************** 
* Local Variable
*****************************************************************************/

/* 
 * Category 420 
 */
static gui_cat420_context_struct *g_mmi_gui_cat420_context_p;

#define CAT420_CTX         (g_mmi_gui_cat420_context_p)

static const U8 g_mmi_gui_cat420_line_style[] = {1, 1, 1, 0, 1, 0};

static UI_filled_area g_mmi_gui_cat420_text_filler = {UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_BORDER,
    NULL,
    NULL,
    {200, 248, 232, 100},
    {0, 0, 0, 0},
    {80, 136, 80, 0},
    {0, 0, 0, 0},
    0
};

/* 
 * Category 425
 */
static gui_cat425_context_struct *g_mmi_gui_cat425_context_p;

#define CAT425_CTX         (g_mmi_gui_cat425_context_p)

#if 0
#endif /* 0 */ 

static const U8 g_mmi_gui_cat425_line_bitvalues[] = {1, 1, 0};

/* 
 * Category 430
 */

static gui_cat430_context_struct *g_mmi_gui_cat430_context_p;

#define CAT430_CTX         (g_mmi_gui_cat430_context_p)

static const UI_filled_area g_mmi_gui_cat430_logbox_filler = 
{
    UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_BORDER,
    NULL,
    NULL,
    {255, 255, 255, 100},
    {0, 0, 0, 0},
    {33, 28, 140, 100},
    {0, 0, 0, 0},
    0
};

static const UI_filled_area g_mmi_gui_cat430_sending_filler = 
{
    UI_FILLED_AREA_TYPE_COLOR,
    NULL,
    NULL,
    {225, 220, 225, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 100},
    {0, 0, 0, 0},
    0
};

/* 
 * Category 435
 */
static gui_cat435_context_struct *g_mmi_gui_cat435_context_p;

#define CAT435_CTX         (g_mmi_gui_cat435_context_p)

static const UI_filled_area g_mmi_gui_cat435_editor_filler = 
{
#ifdef __MMI_TOUCH_SCREEN__
    UI_FILLED_AREA_TYPE_COLOR,  /* No border due to virtual keyboard */
#else 
    UI_FILLED_AREA_TYPE_COLOR | UI_FILLED_AREA_BORDER,
#endif 
    NULL,
    NULL,
    {255, 254, 209, 100},
    {0, 0, 0, 0},
    {184, 184, 184, 100},
    {0, 0, 0, 0},
    0
};

static const UI_filled_area g_mmi_gui_cat435_multitap_filler = {UI_FILLED_AREA_TYPE_COLOR,
    NULL,
    NULL,
    {216, 208, 200, 100},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    0
};

static const color_t g_mmi_gui_cat435_popup_color = {216, 208, 200, 100};

#if defined (__MMI_WALLPAPER_ON_BOTTOM__)
static UI_image_ID_type  editor_scr_bg_ID;
static S8 *editor_scr_bg_filename;
static S32 editor_scr_bg_x, editor_scr_bg_y;
static U8 editor_scr_bg_opacity;
#endif /* defined (__MMI_WALLPAPER_ON_BOTTOM__) */ 

/***************************************************************************** 
* Local Function
*****************************************************************************/

/* 
 * Category 420 
 */


/*****************************************************************************
 * FUNCTION
 *  category420_compute_layout
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category420_compute_layout(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 sw, sh; /* string width/height */
    S32 iw, ih; /* image width/height */
    S32 cw;     /* content width */
    S32 line_height, tmp_y;
    S32 img_left_bound, img_left_lower_bound;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    cw = CAT420_CTX->content_width = UI_device_width - current_MMI_theme->scrollbar_size;

    gui_set_font(&MMI_medium_font);

    /* Name */
    Get_StringWidthHeight((U8*) CAT420_CTX->name, &sw, &sh);
    if (sw > UI_device_width - (CAT420_LINE_EXTRA_PIXEL << 1))
    {
        sw = UI_device_width - (CAT420_LINE_EXTRA_PIXEL << 1);
    }
    CAT420_CTX->name_x = (cw - sw) >> 1;
    CAT420_CTX->name_y = CAT420_TOP_MARGIN;
    CAT420_CTX->name_width = sw;

    CAT420_CTX->line_x1 = CAT420_CTX->name_x - CAT420_LINE_EXTRA_PIXEL;
    CAT420_CTX->line_x2 = CAT420_CTX->name_x + sw + CAT420_LINE_EXTRA_PIXEL - 1;
    if (CAT420_CTX->line_x1 < 0)
    {
        CAT420_CTX->line_x1 = 0;
    }
    if (CAT420_CTX->line_x2 >= cw)
    {
        CAT420_CTX->line_x2 = cw - 1;
    }
    CAT420_CTX->line_y = CAT420_CTX->name_y + sh + CAT420_NAME_LINE_V_SPACING;

    img_left_lower_bound = 0;

    /* Mood */
#ifdef CAT420_DISP_INFO_TEXT
    tmp_y = CAT420_CTX->line_y + CAT420_LINE_IMAGE_V_SPACING;
    Get_StringWidthHeight((U8*) GetString(CAT420_CTX->mood_caption), &sw, &sh);
    gui_measure_image(CAT420_CTX->mood_icon, &iw, &ih);
    CAT420_CTX->mood_caption_x = CAT420_LEFT_MARGIN;
    CAT420_CTX->mood_image_x = CAT420_CTX->mood_caption_x + sw + CAT420_CAPTION_IMAGE_H_SPACING;
    line_height = sh > ih ? sh : ih;
    CAT420_CTX->mood_caption_y = tmp_y + ((line_height - sh) >> 1);
    CAT420_CTX->mood_image_y = tmp_y + ((line_height - ih) >> 1);
    if (CAT420_CTX->mood_image_x + iw > img_left_lower_bound)
    {
        img_left_lower_bound = CAT420_CTX->mood_image_x + iw;
    }
    Get_StringWidthHeight((U8*) CAT420_CTX->mood_string, &sw, &sh);
    CAT420_CTX->mood_text_x = CAT420_LEFT_MARGIN;
    CAT420_CTX->mood_text_y = tmp_y + line_height + CAT420_MOOD_STATUS_V_SPACING;
    if (CAT420_CTX->mood_text_x + sw > img_left_lower_bound)
    {
        img_left_lower_bound = CAT420_CTX->mood_text_x + sw;
    }
#else /* CAT420_DISP_INFO_TEXT */ 
    UI_UNUSED_PARAMETER(line_height);
    tmp_y = CAT420_CTX->line_y + CAT420_LINE_IMAGE_V_SPACING;
    Get_StringWidthHeight((U8*) GetString(CAT420_CTX->mood_caption), &sw, &sh);
    gui_measure_image(CAT420_CTX->mood_icon, &iw, &ih);
    CAT420_CTX->mood_caption_x = CAT420_LEFT_MARGIN;
    CAT420_CTX->mood_caption_y = tmp_y;
    CAT420_CTX->mood_image_x = CAT420_LEFT_MARGIN;
    CAT420_CTX->mood_image_y = tmp_y + sh + CAT420_MOOD_STATUS_V_SPACING;
    if (CAT420_CTX->mood_caption_x + sw > img_left_lower_bound)
    {
        img_left_lower_bound = CAT420_CTX->mood_caption_x + sw;
    }
    if (CAT420_CTX->mood_image_x + iw > img_left_lower_bound)
    {
        img_left_lower_bound = CAT420_CTX->mood_image_x + iw;
    }
#endif /* CAT420_DISP_INFO_TEXT */ 

    /* Status */
#ifdef CAT420_DISP_INFO_TEXT
    tmp_y = CAT420_CTX->mood_text_y + sh + CAT420_MOOD_STATUS_V_SPACING;
    Get_StringWidthHeight((U8*) GetString(CAT420_CTX->status_caption), &sw, &sh);
    gui_measure_image(CAT420_CTX->status_icon, &iw, &ih);
    CAT420_CTX->status_caption_x = CAT420_LEFT_MARGIN;
    CAT420_CTX->status_image_x = CAT420_CTX->status_caption_x + sw + CAT420_CAPTION_IMAGE_H_SPACING;
    line_height = sh > ih ? sh : ih;
    CAT420_CTX->status_caption_y = tmp_y + ((line_height - sh) >> 1);
    CAT420_CTX->status_image_y = tmp_y + ((line_height - ih) >> 1);
    if (CAT420_CTX->status_image_x + iw > img_left_lower_bound)
    {
        img_left_lower_bound = CAT420_CTX->status_image_x + iw;
    }
    Get_StringWidthHeight((U8*) CAT420_CTX->status_string, &sw, &sh);
    CAT420_CTX->status_text_x = CAT420_LEFT_MARGIN;
    CAT420_CTX->status_text_y = tmp_y + line_height + CAT420_MOOD_STATUS_V_SPACING;
    if (CAT420_CTX->status_text_x + sw > img_left_lower_bound)
    {
        img_left_lower_bound = CAT420_CTX->status_text_x + sw;
    }
#else /* CAT420_DISP_INFO_TEXT */ 
    tmp_y = CAT420_CTX->mood_image_y + ih + CAT420_MOOD_STATUS_V_SPACING;
    Get_StringWidthHeight((U8*) GetString(CAT420_CTX->status_caption), &sw, &sh);
    gui_measure_image(CAT420_CTX->status_icon, &iw, &ih);

    CAT420_CTX->status_caption_x = CAT420_LEFT_MARGIN;
    CAT420_CTX->status_caption_y = tmp_y;
    CAT420_CTX->status_image_x = CAT420_LEFT_MARGIN;
    CAT420_CTX->status_image_y = tmp_y + sh + CAT420_MOOD_STATUS_V_SPACING;

    if (CAT420_CTX->status_caption_x + sw > img_left_lower_bound)
    {
        img_left_lower_bound = CAT420_CTX->status_caption_x + sw;
    }
    if (CAT420_CTX->status_image_x + iw > img_left_lower_bound)
    {
        img_left_lower_bound = CAT420_CTX->status_image_x + iw;
    }
#endif /* CAT420_DISP_INFO_TEXT */ 

    /* Compute image width */
    img_left_lower_bound += CAT420_IMAGE_H_SPACING;
    if (img_left_lower_bound > cw - CAT420_RIGHT_MARGIN - CAT420_IMAGE_AREA_MIN_WIDTH)
    {
        img_left_lower_bound = cw - CAT420_RIGHT_MARGIN - CAT420_IMAGE_AREA_MIN_WIDTH;
    }
    img_left_bound = cw - CAT420_RIGHT_MARGIN - CAT420_IMAGE_AREA_WIDTH;
    if (img_left_bound < img_left_lower_bound)
    {
        img_left_bound = img_left_lower_bound;
    }
    img_left_lower_bound -= CAT420_IMAGE_H_SPACING;

    /* Total height */
#ifdef CAT420_DISP_INFO_TEXT
    CAT420_CTX->editor_header_height = CAT420_CTX->status_text_y + sh + CAT420_BOTTOM_MARGIN;
#else 
    CAT420_CTX->editor_header_height = CAT420_CTX->status_image_y + ih + CAT420_BOTTOM_MARGIN;
#endif 

    /* Image */
    if (img_left_bound + CAT420_RIGHT_MARGIN >= cw || CAT420_CTX->image_ori_width == 0 ||
        CAT420_CTX->image_ori_height == 0)
    {
        CAT420_CTX->display_image = MMI_FALSE;
    }

    CAT420_CTX->image_area_x = img_left_bound;
    CAT420_CTX->image_area_y = CAT420_CTX->line_y + CAT420_LINE_IMAGE_V_SPACING;
    CAT420_CTX->image_area_width = cw - CAT420_RIGHT_MARGIN - img_left_bound;
#ifdef CAT420_DISP_INFO_TEXT
    CAT420_CTX->image_area_height = CAT420_CTX->status_text_y + sh - CAT420_CTX->image_area_y;
#else 
    CAT420_CTX->image_area_height = CAT420_CTX->status_image_y + ih - CAT420_CTX->image_area_y;
#endif 

    /* Align text/icon to right for Arabic language */
    if (r2lMMIFlag)
    {
        S32 xd;

        /* Align mood to right */
        xd = CAT420_CTX->mood_caption_x - CAT420_LEFT_MARGIN;
        sw = gui_get_string_width((UI_string_type) GetString(CAT420_CTX->mood_caption));
        CAT420_CTX->mood_caption_x = img_left_lower_bound - xd - sw + 1;

        xd = CAT420_CTX->mood_image_x - CAT420_LEFT_MARGIN;
        gui_measure_image(CAT420_CTX->mood_icon, &iw, &ih);
        CAT420_CTX->mood_image_x = img_left_lower_bound - xd - iw + 1;

    #ifdef CAT420_DISP_INFO_TEXT
        xd = CAT420_CTX->mood_text_x - CAT420_LEFT_MARGIN;
        sw = gui_get_string_width(CAT420_CTX->mood_string);
        CAT420_CTX->mood_text_x = img_left_lower_bound - xd - sw + 1;
    #endif /* CAT420_DISP_INFO_TEXT */ 

        /* Align status to right */
        xd = CAT420_CTX->status_caption_x - CAT420_LEFT_MARGIN;
        sw = gui_get_string_width((UI_string_type) GetString(CAT420_CTX->status_caption));
        CAT420_CTX->status_caption_x = img_left_lower_bound - xd - sw + 1;

        xd = CAT420_CTX->status_image_x - CAT420_LEFT_MARGIN;
        gui_measure_image(CAT420_CTX->status_icon, &iw, &ih);
        CAT420_CTX->status_image_x = img_left_lower_bound - xd - iw + 1;

    #ifdef CAT420_DISP_INFO_TEXT
        xd = CAT420_CTX->status_text_x - CAT420_LEFT_MARGIN;
        sw = gui_get_string_width(CAT420_CTX->status_string);
        CAT420_CTX->status_text_x = img_left_lower_bound - xd - sw + 1;
    #endif /* CAT420_DISP_INFO_TEXT */ 

    }

    /* Resize photo if required */
    if (CAT420_CTX->display_image)
    {
        S32 zoom_offset_x, zoom_offset_y, zoom_size_width, zoom_size_height;

        if (CAT420_CTX->image_ori_width <= CAT420_CTX->image_area_width &&
            CAT420_CTX->image_ori_height <= CAT420_CTX->image_area_height)
        {
            zoom_offset_x = (CAT420_CTX->image_area_width - CAT420_CTX->image_ori_width) >> 1;
            zoom_offset_y = (CAT420_CTX->image_area_height - CAT420_CTX->image_ori_height) >> 1;
            zoom_size_width = 0;
            zoom_size_height = 0;
        }
        else
        {
            gdi_image_util_fit_bbox(
                (S32) CAT420_CTX->image_area_width,
                (S32) CAT420_CTX->image_area_height,
                (S32) CAT420_CTX->image_ori_width,
                (S32) CAT420_CTX->image_ori_height,
                &zoom_offset_x,
                &zoom_offset_y,
                &zoom_size_width,
                &zoom_size_height);
        }
        CAT420_CTX->image_x_offset = (S16) CAT420_CTX->image_area_x + zoom_offset_x;
        CAT420_CTX->image_y_offset = (S16) CAT420_CTX->image_area_y + zoom_offset_y;
        CAT420_CTX->image_resized_width = (S16) zoom_size_width;
        CAT420_CTX->image_resized_height = (S16) zoom_size_height;
    }
}

/* FIXME. We may move image caching routines to wgui.c */


/*****************************************************************************
 * FUNCTION
 *  category420_cache_image
 * DESCRIPTION
 *  
 * PARAMETERS
 *  image_path                  [?]         
 *  alternative_image           [?]         
 *  use_alternative_image       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void category420_cache_image(S8 *image_path, U8 *alternative_image, S32 use_alternative_image)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!CAT420_CTX->display_image)
    {
        CAT420_CTX->image_is_cached = 0;
        CAT420_CTX->cache_layer = GDI_LAYER_EMPTY_HANDLE;
    }
    else
    {
        CAT420_CTX->image_is_cached = 1;
        gdi_layer_multi_layer_enable();
        gdi_layer_create(0, 0, UI_device_width, UI_device_height, &CAT420_CTX->cache_layer);
        gdi_layer_push_and_set_active(CAT420_CTX->cache_layer);

        if (CAT420_CTX->image_resized_width == 0 || CAT420_CTX->image_resized_height == 0)
        {
            /* Fill background in case that the image is truncated */
            gdi_draw_solid_rect(
                0,
                0,
                CAT420_CTX->image_ori_width,
                CAT420_CTX->image_ori_height,
                gdi_act_color_from_rgb(255, 255, 255, 255));
            if (use_alternative_image)
            {
                gdi_image_draw(0, 0, alternative_image);
            }
            else
            {
                gdi_image_draw_file(0, 0, image_path);
            }
        }
        else
        {
            gdi_draw_solid_rect(
                0,
                0,
                CAT420_CTX->image_resized_width,
                CAT420_CTX->image_resized_height,
                gdi_act_color_from_rgb(255, 255, 255, 255));
            if (use_alternative_image)
            {
                gdi_image_draw_resized(
                    0,
                    0,
                    CAT420_CTX->image_resized_width,
                    CAT420_CTX->image_resized_height,
                    alternative_image);
            }
            else
            {
                gdi_image_draw_resized_file(
                    0,
                    0,
                    CAT420_CTX->image_resized_width,
                    CAT420_CTX->image_resized_height,
                    image_path);
            }
        }

        gdi_layer_pop_and_restore_active();
    }
}

/* Free GDI layer of the image cache */


/*****************************************************************************
 * FUNCTION
 *  category420_free_image_cache
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category420_free_image_cache(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (CAT420_CTX->image_is_cached)
    {
        gdi_layer_free(CAT420_CTX->cache_layer);
        gdi_layer_multi_layer_disable();
    }
}


/*****************************************************************************
 * FUNCTION
 *  category420_show_cached_image
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x_offset        [IN]        
 *  y_offset        [IN]        
 *  width           [IN]        
 *  height          [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
	extern GDI_RESULT gdi_layer_get_buffer_ptr(U8 **buf_ptr);
static void category420_show_cached_image(S32 x_offset, S32 y_offset, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, x, y, y_multiple;
    U8 *cached_buffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!CAT420_CTX->image_is_cached)
    {
        return;
    }

    /* Get clipping region inside the editor */
    gdi_layer_get_clip(&x1, &y1, &x2, &y2);
    gdi_layer_push_and_set_active(CAT420_CTX->cache_layer);
    gdi_layer_get_buffer_ptr(&cached_buffer);
    gdi_layer_pop_and_restore_active();

    if (x1 < x_offset)
    {
        x1 = x_offset;
    }
    if (y1 < y_offset)
    {
        y1 = y_offset;
    }
    if (x2 > x_offset + width - 1)
    {
        x2 = x_offset + width - 1;
    }
    if (y2 > y_offset + height - 1)
    {
        y2 = y_offset + height - 1;
    }

    y_multiple = (y1 - y_offset) * UI_device_width;
    for (y = y1; y <= y2; y++)
    {
        for (x = x1; x <= x2; x++)
        {
            GDI_SET_BUFFER_PIXEL(x, y, cached_buffer[y_multiple + x - x_offset]);
        }
        y_multiple += UI_device_width;
    }
}


/*****************************************************************************
 * FUNCTION
 *  category420_header_callback
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
static void category420_header_callback(S32 yoffset, S32 clip_x1, S32 clip_y1, S32 clip_x2, S32 clip_y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 sw, sh; /* string width/height */
    color_t tc;
    gdi_color c;
    S32 xoffset = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Hack. xoffset should be provided in callback function */
    if (r2lMMIFlag)
    {
        xoffset = MMI_multiline_inputbox.vbar.width;
    }

    gui_push_clip();
    gui_push_text_clip();
    gui_set_clip(clip_x1, clip_y1, clip_x2, clip_y2);
    gui_set_text_clip(clip_x1, clip_y1, clip_x2, clip_y2);
    gui_set_font(&MMI_medium_font);

    tc.r = CAT420_TEXT_COLOR_R;
    tc.g = CAT420_TEXT_COLOR_G;
    tc.b = CAT420_TEXT_COLOR_B;
    tc.alpha = 100;
    gui_set_text_color(tc);

    c = gdi_act_color_from_rgb(255, CAT420_LINE_COLOR_R, CAT420_LINE_COLOR_G, CAT420_LINE_COLOR_B);

    /* Name */
    Get_StringWidthHeight((U8*) CAT420_CTX->name, &sw, &sh);
    if (sw > CAT420_CTX->name_width)
    {
        if (r2lMMIFlag)
        {
            gui_print_truncated_text(
                xoffset + CAT420_CTX->name_x + CAT420_CTX->name_width - 1,
                yoffset + CAT420_CTX->name_y,
                CAT420_CTX->name_width,
                CAT420_CTX->name);
        }
        else
        {
            gui_print_truncated_text(
                xoffset + CAT420_CTX->name_x,
                yoffset + CAT420_CTX->name_y,
                CAT420_CTX->name_width,
                CAT420_CTX->name);
        }
    }
    else
    {
        if (r2lMMIFlag)
        {
            gui_move_text_cursor(xoffset + CAT420_CTX->name_x + sw - 1, yoffset + CAT420_CTX->name_y);
        }
        else
        {
            gui_move_text_cursor(xoffset + CAT420_CTX->name_x, yoffset + CAT420_CTX->name_y);
        }
        gui_print_text(CAT420_CTX->name);
    }

    gdi_draw_line_style(
        xoffset + CAT420_CTX->line_x1,
        yoffset + CAT420_CTX->line_y,
        xoffset + CAT420_CTX->line_x2,
        yoffset + CAT420_CTX->line_y,
        c,
        sizeof(g_mmi_gui_cat420_line_style),
        g_mmi_gui_cat420_line_style);

    /* Mood */
    sw = gui_get_string_width((UI_string_type) GetString(CAT420_CTX->mood_caption));
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(xoffset + CAT420_CTX->mood_caption_x + sw - 1, yoffset + CAT420_CTX->mood_caption_y);
    }
    else
    {
        gui_move_text_cursor(xoffset + CAT420_CTX->mood_caption_x, yoffset + CAT420_CTX->mood_caption_y);
    }
    gui_print_text((UI_string_type) GetString(CAT420_CTX->mood_caption));
    gui_show_transparent_image(
        xoffset + CAT420_CTX->mood_image_x,
        yoffset + CAT420_CTX->mood_image_y,
        CAT420_CTX->mood_icon,
        UI_DEFAULT_TRANSPARENT_COLOR);
#ifdef CAT420_DISP_INFO_TEXT
    sw = gui_get_string_width(CAT420_CTX->mood_string);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(xoffset + CAT420_CTX->mood_text_x + sw - 1, yoffset + CAT420_CTX->mood_text_y);
    }
    else
    {
        gui_move_text_cursor(xoffset + CAT420_CTX->mood_text_x, yoffset + CAT420_CTX->mood_text_y);
    }
    gui_print_text(CAT420_CTX->mood_string);
#endif /* CAT420_DISP_INFO_TEXT */ 

    /* Status */
    sw = gui_get_string_width((UI_string_type) GetString(CAT420_CTX->status_caption));
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(xoffset + CAT420_CTX->status_caption_x + sw - 1, yoffset + CAT420_CTX->status_caption_y);
    }
    else
    {
        gui_move_text_cursor(xoffset + CAT420_CTX->status_caption_x, yoffset + CAT420_CTX->status_caption_y);
    }
    gui_print_text((UI_string_type) GetString(CAT420_CTX->status_caption));
    gui_show_transparent_image(
        xoffset + CAT420_CTX->status_image_x,
        yoffset + CAT420_CTX->status_image_y,
        CAT420_CTX->status_icon,
        UI_DEFAULT_TRANSPARENT_COLOR);
#ifdef CAT420_DISP_INFO_TEXT
    sw = gui_get_string_width(CAT420_CTX->status_string);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(xoffset + CAT420_CTX->status_text_x + sw - 1, yoffset + CAT420_CTX->status_text_y);
    }
    else
    {
        gui_move_text_cursor(xoffset + CAT420_CTX->status_text_x, yoffset + CAT420_CTX->status_text_y);
    }
    gui_print_text(CAT420_CTX->status_string);
#endif /* CAT420_DISP_INFO_TEXT */ 

    /* Image */
    if (CAT420_CTX->display_image)
    {
        if (CAT420_CTX->image_resized_width == 0 || CAT420_CTX->image_resized_height == 0)
        {
            category420_show_cached_image(
                xoffset + CAT420_CTX->image_x_offset,
                yoffset + CAT420_CTX->image_y_offset,
                CAT420_CTX->image_ori_width,
                CAT420_CTX->image_ori_height);
        }
        else
        {
            category420_show_cached_image(
                xoffset + CAT420_CTX->image_x_offset,
                yoffset + CAT420_CTX->image_y_offset,
                CAT420_CTX->image_resized_width,
                CAT420_CTX->image_resized_height);
        }
    }
    gdi_draw_rect(
        xoffset + CAT420_CTX->image_area_x - 1,
        yoffset + CAT420_CTX->image_area_y - 1,
        xoffset + CAT420_CTX->image_area_x + CAT420_CTX->image_area_width,
        yoffset + CAT420_CTX->image_area_y + CAT420_CTX->image_area_height,
        c);

    gui_pop_text_clip();
    gui_pop_clip();
}

/* 
 * Category 425
 */


/*****************************************************************************
 * FUNCTION
 *  category425_set_icontext_list_text_color
 * DESCRIPTION
 *  
 * PARAMETERS
 *  col     [IN]        
 *  c1      [IN]        
 *  c2      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void category425_set_icontext_list_text_color(int col, color_t c1, color_t c2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_icontext_list_menuitem.normal_text_colors[col] = c1;
    MMI_fixed_icontext_list_menuitem.disabled_text_colors[col] = c1;
    MMI_fixed_icontext_list_menuitem.selected_text_colors[col] = c2;
    MMI_fixed_icontext_list_menuitem.focussed_text_colors[col] = c2;
}


/*****************************************************************************
 * FUNCTION
 *  category425_setup_text_color
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category425_setup_text_color(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c1, c2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MAINLCD_240X320__
    c1 = *current_MMI_theme->list_normal_text_color;
    c2 = *current_MMI_theme->list_selected_text_color;
#else /* __MMI_MAINLCD_240X320__ */ 
    c1.r = CAT425_TEXT1_COLOR_R;
    c1.g = CAT425_TEXT1_COLOR_G;
    c1.b = CAT425_TEXT1_COLOR_B;
    c2.r = CAT425_TEXT1_HIGHLIGHT_COLOR_R;
    c2.g = CAT425_TEXT1_HIGHLIGHT_COLOR_G;
    c2.b = CAT425_TEXT1_HIGHLIGHT_COLOR_B;
#endif /* __MMI_MAINLCD_240X320__ */ 
    category425_set_icontext_list_text_color(0, c1, c2);

#ifndef __MMI_MAINLCD_240X320__
    c1.r = CAT425_TEXT2_COLOR_R;
    c1.g = CAT425_TEXT2_COLOR_G;
    c1.b = CAT425_TEXT2_COLOR_B;
    c2.r = CAT425_TEXT2_HIGHLIGHT_COLOR_R;
    c2.g = CAT425_TEXT2_HIGHLIGHT_COLOR_G;
    c2.b = CAT425_TEXT2_HIGHLIGHT_COLOR_B;
#endif /* __MMI_MAINLCD_240X320__ */ 
    category425_set_icontext_list_text_color(1, c1, c2);
}


/*****************************************************************************
 * FUNCTION
 *  category425_reset_context
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category425_reset_context(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_fixed_list_menu.flags = CAT425_CTX->backup_menu_flags;
    MMI_fixed_list_menu.normal_filler = CAT425_CTX->backup_menu_filler;
    fixed_icontext_list_menuitem_apply_current_theme();

    OslMfree(g_mmi_gui_cat425_context_p);
    g_mmi_gui_cat425_context_p = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  category425_post_display_menuitem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item                    [?]         
 *  common_item_data        [?]         
 *  x                       [IN]        
 *  y                       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void category425_post_display_menuitem(void *item, void *common_item_data, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    color_t c;
    fixed_icontext_list_menuitem *m = (fixed_icontext_list_menuitem*) common_item_data;
    fixed_icontext_list_menuitem_type *mi = (fixed_icontext_list_menuitem_type*) item;
    S32 x2, y2;
    gdi_color border_color;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    x2 = x + m->width - 1;
    y2 = y + m->height - 1;

    gui_push_clip();
    gui_set_clip(x, y, x2, y2);

    border_color =
        (gdi_color) ((mi->flags & UI_MENUITEM_STATE_FOCUSSED) ?
                     gdi_act_color_from_rgb(
                        255,
                        CAT425_ICON_HIGHLIGHT_BORDER_COLOR_R,
                        CAT425_ICON_HIGHLIGHT_BORDER_COLOR_G,
                        CAT425_ICON_HIGHLIGHT_BORDER_COLOR_B) : gdi_act_color_from_rgb(
                                                                                        255,
                                                                                        CAT425_ICON_BORDER_COLOR_R,
                                                                                        CAT425_ICON_BORDER_COLOR_G,
                                                                                        CAT425_ICON_BORDER_COLOR_B));

    c.r = CAT425_TEXT2_BRACKET_COLOR_R;
    c.g = CAT425_TEXT2_BRACKET_COLOR_G;
    c.b = CAT425_TEXT2_BRACKET_COLOR_B;
    gui_set_text_color(c);
    if (r2lMMIFlag)
    {
        gui_move_text_cursor(x2 - CAT425_CTX->bracket_x, y + CAT425_CTX->bracket_y);
        gui_print_text((UI_string_type) L"]");
        gui_move_text_cursor(
            x2 - m->text_coordinates[1].x - m->text_coordinates[1].width - CAT425_TEXT2_BRACKET_GAP,
            y + CAT425_CTX->bracket_y);
        gui_print_text((UI_string_type) L"[");

        gdi_draw_rect(
            x2 - CAT425_CTX->icon_border_x1,
            y + CAT425_CTX->icon_border_y1,
            x2 - CAT425_CTX->icon_border_x2,
            y + CAT425_CTX->icon_border_y2,
            border_color);
    }
    else
    {
        gui_move_text_cursor(x + CAT425_CTX->bracket_x, y + CAT425_CTX->bracket_y);
        gui_print_text((UI_string_type) L"[");
        gui_move_text_cursor(
            x + m->text_coordinates[1].x + m->text_coordinates[1].width + CAT425_TEXT2_BRACKET_GAP,
            y + CAT425_CTX->bracket_y);
        gui_print_text((UI_string_type) L"]");

        gdi_draw_rect(
            x + CAT425_CTX->icon_border_x1,
            y + CAT425_CTX->icon_border_y1,
            x + CAT425_CTX->icon_border_x2,
            y + CAT425_CTX->icon_border_y2,
            border_color);
    }

    gdi_draw_line_style(
        x,
        y2,
        x2,
        y2,
        gdi_act_color_from_rgb(255, CAT425_LINE_COLOR_R, CAT425_LINE_COLOR_G, CAT425_LINE_COLOR_B),
        sizeof(g_mmi_gui_cat425_line_bitvalues),
        g_mmi_gui_cat425_line_bitvalues);

    gui_pop_clip();
}

/* 
 * Category 430
 */

/* b is configured, and b->height is computed */


/*****************************************************************************
 * FUNCTION
 *  category430_create_sending_box
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]         
 *  x       [IN]        
 *  y       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void category430_create_sending_box(multi_line_input_box *b, S16 x, S16 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 length;
    S32 read_index;
    wgui_color_buffer_writer_struct writer;
    S32 h;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    length = gui_strlen((UI_string_type) CAT430_CTX->sending_buffer);

    /* Create color text buffer */
    MMI_ASSERT(CAT430_CTX->sent_count <= length && length <= CAT430_CTX->max_sending_buffer_size);

    CAT430_CTX->allocated_color_buffer[0] = CAT430_CTX->allocated_color_buffer[1] = 0;
    wgui_color_buffer_create_writer(
        &writer,
        CAT430_CTX->allocated_color_buffer,
        CAT430_CTX->allocated_color_buffer_size);
    wgui_color_buffer_write_char(&writer, GUI_INPUT_COLOR_BLACK);
    for (read_index = 0; read_index < CAT430_CTX->sent_count; read_index++)
    {
        wgui_color_buffer_write_char(&writer, *((U16*) & CAT430_CTX->sending_buffer[read_index << 1]));
    }
    wgui_color_buffer_write_char(&writer, GUI_INPUT_COLOR_TEAL);
    for (read_index = CAT430_CTX->sent_count; read_index < length; read_index++)
    {
        wgui_color_buffer_write_char(&writer, *((U16*) & CAT430_CTX->sending_buffer[read_index << 1]));
    }

    gui_create_multi_line_input_box_set_buffer(
        b,
        x,
        y,
        CAT430_CTX->sending_box_width,
        UI_device_height   ,
        (UI_string_type) CAT430_CTX->allocated_color_buffer,
        CAT430_CTX->max_sending_buffer_size * 2,
        (length + 1) * 2,
        0);

    b->flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE |
         UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR | UI_MULTI_LINE_INPUT_BOX_RICHTEXT);
    b->normal_filler = (UI_filled_area*)&g_mmi_gui_cat430_sending_filler;

    /* Compute text height and adjust scroll offset */
    b->flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
    gui_show_multi_line_input_box(b);
    b->flags &= ~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;

    /* Setup height */
    h = b->text_height + b->text_y;
    gui_resize_multi_line_input_box(b, b->width, h);
}


/*****************************************************************************
 * FUNCTION
 *  category430_setup_sending_box
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category430_setup_sending_box(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    multi_line_input_box b;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (CAT430_CTX->show_sending_buffer)
    {
        CAT430_CTX->sending_box_x = CAT430_SENDING_BOX_H_GAP;
        CAT430_CTX->sending_box_y = CAT430_SENDING_BOX_V_GAP;
        CAT430_CTX->sending_box_width = CAT430_CTX->log_box2_width -
            2 * CAT430_SENDING_BOX_H_GAP - current_multi_line_input_box_theme->vbar_width;
        category430_create_sending_box(&b, 0, 0);
        CAT430_CTX->log_box2_footer_height = b.height + 2 * CAT430_SENDING_BOX_V_GAP;
    }
    else
    {
        CAT430_CTX->log_box2_footer_height = 0;
    }
}

#ifdef CAT430_NAME_IN_LOG


/*****************************************************************************
 * FUNCTION
 *  category430_header1_callback
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
static void category430_header1_callback(S32 yoffset, S32 clip_x1, S32 clip_y1, S32 clip_x2, S32 clip_y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 sw;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_set_clip(clip_x1, clip_y1, clip_x2, clip_y2);

    gui_set_font(CAT430_NAME_FONT);
    sw = gui_get_string_width((UI_string_type) CAT430_CTX->name1);

    if (r2lMMIFlag)
    {
        gui_move_text_cursor(CAT430_CTX->log_box1_x + CAT430_HORIZONTAL_GAP + sw - 1, yoffset);
    }
    else
    {
        gui_move_text_cursor(CAT430_CTX->log_box1_x + CAT430_HORIZONTAL_GAP, yoffset);
    }
    gui_set_text_color(gui_color(CAT430_NAME1_COLOR_R, CAT430_NAME1_COLOR_G, CAT430_NAME1_COLOR_B));
    gui_print_text((UI_string_type) CAT430_CTX->name1);

    gui_pop_clip();

}


/*****************************************************************************
 * FUNCTION
 *  category430_header2_callback
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
static void category430_header2_callback(S32 yoffset, S32 clip_x1, S32 clip_y1, S32 clip_x2, S32 clip_y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 sw;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_set_clip(clip_x1, clip_y1, clip_x2, clip_y2);

    gui_set_font(CAT430_NAME_FONT);
    sw = gui_get_string_width((UI_string_type) CAT430_CTX->name2);

    if (r2lMMIFlag)
    {
        gui_move_text_cursor(CAT430_CTX->log_box2_x + CAT430_HORIZONTAL_GAP + sw - 1, yoffset);
    }
    else
    {
        gui_move_text_cursor(CAT430_CTX->log_box2_x + CAT430_HORIZONTAL_GAP, yoffset);
    }
    gui_set_text_color(gui_color(CAT430_NAME2_COLOR_R, CAT430_NAME2_COLOR_G, CAT430_NAME2_COLOR_B));
    gui_print_text((UI_string_type) CAT430_CTX->name2);

    gui_pop_clip();
}

#endif /* CAT430_NAME_IN_LOG */ 


/*****************************************************************************
 * FUNCTION
 *  category430_footer_callback
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
static void category430_footer_callback(S32 yoffset, S32 clip_x1, S32 clip_y1, S32 clip_x2, S32 clip_y2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    multi_line_input_box b;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    category430_create_sending_box(&b, (S16) CAT430_CTX->sending_box_x, (S16) (yoffset + CAT430_CTX->sending_box_y));
    b.flags |= UI_MULTI_LINE_INPUT_BOX_USE_PRESET_CLIPPING;
    gui_push_clip();
    gui_set_clip(clip_x1, clip_y1, clip_x2, clip_y2);
    gui_show_multi_line_input_box(&b);
    gui_pop_clip();
}


/*****************************************************************************
 * FUNCTION
 *  category430_setup_log_box1
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category430_setup_log_box1(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*
     * In Arabic, the ailgnment format is decided by the content of file. 
     * * MSN align chat log to left.
     */
    S32 log_length;
    multi_line_input_box *b = &CAT430_CTX->log_box1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    log_length = gui_strlen((UI_string_type) CAT430_CTX->log_buffer1);
    gui_create_multi_line_input_box_set_buffer(
        b,
        CAT430_CTX->log_box1_x,
        CAT430_CTX->log_box1_y,
        CAT430_CTX->log_box1_width,
        CAT430_CTX->log_box1_height,
        (UI_string_type) CAT430_CTX->log_buffer1,
        CAT430_CTX->log_buffer_size1 * 2,
        (log_length + 1) * 2,
        0);

#ifdef CAT430_NAME_IN_LOG
    b->header_height = CAT430_CTX->log_box_header_height;
    b->header_callback = category430_header1_callback;
#endif /* CAT430_NAME_IN_LOG */ 

    b->flags |= (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE);
    b->normal_filler = (UI_filled_area*)&g_mmi_gui_cat430_logbox_filler;

    /* Compute text height and adjust scroll offset */
    b->flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
    gui_show_multi_line_input_box(b);
    b->flags &= ~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
    if (b->text_height > b->edit_height)
    {
        b->text_offset_y = -(b->text_height - b->edit_height);
    }
}


/*****************************************************************************
 * FUNCTION
 *  category430_setup_log_box2
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category430_setup_log_box2(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*
     * In Arabic, the ailgnment format is decided by the content of file. 
     * * MSN align chat log to left.
     */
    S32 log_length;
    multi_line_input_box *b = &MMI_multiline_inputbox;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    log_length = gui_strlen((UI_string_type) CAT430_CTX->log_buffer2);
    gui_create_multi_line_input_box_set_buffer(
        b,
        CAT430_CTX->log_box2_x,
        CAT430_CTX->log_box2_y,
        CAT430_CTX->log_box2_width,
        CAT430_CTX->log_box2_height,
        (UI_string_type) CAT430_CTX->log_buffer2,
        CAT430_CTX->log_buffer_size2 * 2,
        (log_length + 1) * 2,
        0);

#ifdef CAT430_NAME_IN_LOG
    b->header_height = CAT430_CTX->log_box_header_height;
    b->header_callback = category430_header2_callback;
#endif /* CAT430_NAME_IN_LOG */ 
    b->footer_height = CAT430_CTX->log_box2_footer_height;
    b->footer_callback = category430_footer_callback;
    b->max_empty_text_height = 1;   /* For the case sending box is not empty but log box2 is empty. */

    b->flags |= (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE);
    b->normal_filler = (UI_filled_area*)&g_mmi_gui_cat430_logbox_filler;

    /* Compute text height and adjust scroll offset */
    b->flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
    gui_show_multi_line_input_box(b);
    b->flags &= ~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
    if (b->text_height > b->edit_height)
    {
        b->text_offset_y = -(b->text_height - b->edit_height);
    }
}


/*****************************************************************************
 * FUNCTION
 *  category430_draw_content_area
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void category430_draw_content_area(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifndef CAT430_NAME_IN_LOG
    S32 sw;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifndef __MMI_WALLPAPER_ON_BOTTOM__
    gui_draw_filled_area(
        MMI_content_x,
        MMI_content_y,
        MMI_content_x + MMI_content_width - 1,
        MMI_content_y + MMI_content_height - 1,
        MMI_fixed_list_menu.normal_filler);
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 

#ifndef CAT430_NAME_IN_LOG
    gui_set_font(CAT430_NAME_FONT);
    sw = gui_get_string_width((UI_string_type) CAT430_CTX->name1);

    if (r2lMMIFlag)
    {
        gui_move_text_cursor(CAT430_CTX->name1_x + sw - 1, CAT430_CTX->name1_y);
    }
    else
    {
        gui_move_text_cursor(CAT430_CTX->name1_x, CAT430_CTX->name1_y);
    }
    gui_set_text_color(gui_color(CAT430_NAME1_COLOR_R, CAT430_NAME1_COLOR_G, CAT430_NAME1_COLOR_B));
    gui_print_text((UI_string_type) CAT430_CTX->name1);

    sw = gui_get_string_width((UI_string_type) CAT430_CTX->name2);

    if (r2lMMIFlag)
    {
        gui_move_text_cursor(CAT430_CTX->name2_x + sw - 1, CAT430_CTX->name2_y);
    }
    else
    {
        gui_move_text_cursor(CAT430_CTX->name2_x, CAT430_CTX->name2_y);
    }
    gui_set_text_color(gui_color(CAT430_NAME2_COLOR_R, CAT430_NAME2_COLOR_G, CAT430_NAME2_COLOR_B));
    gui_print_text((UI_string_type) CAT430_CTX->name2);
#endif /* CAT430_NAME_IN_LOG */ 

    gui_show_multi_line_input_box(&CAT430_CTX->log_box1);
    gui_show_multi_line_input_box(&MMI_multiline_inputbox);
}


/*****************************************************************************
 * FUNCTION
 *  category430_previous_line
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void category430_previous_line(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_multi_line_input_box_previous_line(&CAT430_CTX->log_box1);
    gui_show_multi_line_input_box(&CAT430_CTX->log_box1);
    gui_multi_line_input_box_previous_line(&MMI_multiline_inputbox);
    gui_show_multi_line_input_box(&MMI_multiline_inputbox);
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  category430_next_line
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void category430_next_line(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_multi_line_input_box_next_line(&CAT430_CTX->log_box1);
    gui_show_multi_line_input_box(&CAT430_CTX->log_box1);
    gui_multi_line_input_box_next_line(&MMI_multiline_inputbox);
    gui_show_multi_line_input_box(&MMI_multiline_inputbox);
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  category430_draw_controlled_area
 * DESCRIPTION
 *  
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void category430_draw_controlled_area(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    category430_draw_content_area();
}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  category430_control_area_pen_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point           [IN]        
 *  pen_event       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category430_control_area_pen_handler(mmi_pen_point_struct point, mmi_pen_event_type_enum pen_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_multi_line_input_box_pen_enum event;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!CAT430_CTX)
    {
        MMI_DBG_ASSERT(0);
        return MMI_FALSE;
    }

    if (pen_event == MMI_PEN_EVENT_DOWN)
    {
        CAT430_CTX->pen_object = 0;

        if (gui_multi_line_input_box_translate_pen_event(&CAT430_CTX->log_box1, pen_event, point.x, point.y, &event))
        {
            CAT430_CTX->pen_object = 1;
            return MMI_TRUE;
        }
        if (gui_multi_line_input_box_translate_pen_event(&MMI_multiline_inputbox, pen_event, point.x, point.y, &event))
        {
            CAT430_CTX->pen_object = 2;
            return MMI_TRUE;
        }
        else
        {
            return MMI_FALSE;
        }
    }
    else
    {
        switch (CAT430_CTX->pen_object)
        {
            case 1:
                return gui_multi_line_input_box_translate_pen_event(
                        &CAT430_CTX->log_box1,
                        pen_event,
                        point.x,
                        point.y,
                        &event);
            case 2:
                return gui_multi_line_input_box_translate_pen_event(
                        &MMI_multiline_inputbox,
                        pen_event,
                        point.x,
                        point.y,
                        &event);
            default:
                return MMI_TRUE;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  category430_control_area_pen_down_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category430_control_area_pen_down_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return category430_control_area_pen_handler(point, MMI_PEN_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  category430_control_area_pen_move_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category430_control_area_pen_move_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return category430_control_area_pen_handler(point, MMI_PEN_EVENT_MOVE);
}


/*****************************************************************************
 * FUNCTION
 *  category430_control_area_pen_longtap_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category430_control_area_pen_longtap_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return category430_control_area_pen_handler(point, MMI_PEN_EVENT_LONG_TAP);
}


/*****************************************************************************
 * FUNCTION
 *  category430_control_area_pen_repeat_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category430_control_area_pen_repeat_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return category430_control_area_pen_handler(point, MMI_PEN_EVENT_REPEAT);
}


/*****************************************************************************
 * FUNCTION
 *  category430_control_area_pen_up_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category430_control_area_pen_up_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return category430_control_area_pen_handler(point, MMI_PEN_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  category430_control_area_pen_abort_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category430_control_area_pen_abort_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return category430_control_area_pen_handler(point, MMI_PEN_EVENT_ABORT);
}

#endif /* __MMI_TOUCH_SCREEN__ */ 

/* 
 * Category 435
 */


/*****************************************************************************
 * FUNCTION
 *  category435_hide_multitap
 * DESCRIPTION
 *  Hide function for multitap
 *  
 *  Internal function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category435_hide_multitap(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* We fill the multitap bar across the screen instead of using MMI_multitap_x & MMI_multitap_width */
    gui_lock_double_buffer();
    gui_reset_clip();
    /* HACK. IME might clear the background. We can not use MMI_multitap_x & MMI_multitap_width to erase background */
    gui_fill_rectangle(
        0,
        MMI_multitap_y,
        UI_device_width - 1,
        MMI_multitap_y + MMI_multitap_height - 1,
        g_mmi_gui_cat435_popup_color);
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, MMI_multitap_y, UI_device_width - 1, MMI_multitap_y + MMI_multitap_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  category435_setup_log_box
 * DESCRIPTION
 *  
 * PARAMETERS
 *  log_buffer          [?]         
 *  log_buffer_size     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void category435_setup_log_box(U8 *log_buffer, S32 log_buffer_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*
     * In Arabic, the ailgnment format is decided by the content of file. 
     * * MSN align chat log to left.
     */
    S32 log_length;
    multi_line_input_box *b = &CAT435_CTX->log_box;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    log_length = gui_strlen((UI_string_type) log_buffer);
    gui_create_multi_line_input_box_set_buffer(
        b,
        MMI_CONTENT_X_WITH_H_TAB,
        MMI_CONTENT_Y_WITH_H_TAB,
        MMI_CONTENT_WIDTH_WITH_H_TAB,
        MMI_CONTENT_HEIGHT_WITH_H_TAB,
        (UI_string_type) log_buffer,
        log_buffer_size * 2,
        (log_length + 1) * 2,
        0);
    gui_move_multi_line_input_box(b, MMI_CONTENT_X_WITH_H_TAB, MMI_CONTENT_Y_WITH_H_TAB);
    gui_resize_multi_line_input_box(b, MMI_CONTENT_WIDTH_WITH_H_TAB, MMI_CONTENT_HEIGHT_WITH_H_TAB);

    b->flags |=
        (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE |
         UI_MULTI_LINE_INPUT_BOX_RICHTEXT);
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    b->flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
#endif 
    b->icon_callback = CAT435_CTX->icon_callback;
    // b->min_line_height = (S16) CAT435_CTX->icon_height + 1; /* Editor has one more pixel between lines */
    // b->normal_filler = (UI_filled_area *)&g_mmi_gui_cat435_editor_filler;

    // TODO: combine the following two procedure in one function in gui_inputs.c
    /* Compute text height and adjust scroll offset */
    b->flags |= UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
    gui_show_multi_line_input_box(b);
    b->flags &= ~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
    if (b->text_height > b->edit_height)
    {
        b->text_offset_y = -(b->text_height - b->edit_height);
    }
}


/*****************************************************************************
 * FUNCTION
 *  category435_log_box_previous_line
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void category435_log_box_previous_line(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_multi_line_input_box_previous_line(&CAT435_CTX->log_box);
    gui_show_multi_line_input_box(&CAT435_CTX->log_box);
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  category435_log_box_next_line
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void category435_log_box_next_line(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    gui_multi_line_input_box_next_line(&CAT435_CTX->log_box);
    gui_show_multi_line_input_box(&CAT435_CTX->log_box);
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}

#if defined(__MMI_TOUCH_SCREEN__)


/*****************************************************************************
 * FUNCTION
 *  cat435_virtual_keypad_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void cat435_virtual_keypad_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_pen_editor_resize_multiline_input_box_for_vk();
}
#endif /* defined(__MMI_TOUCH_SCREEN__) */ 


/*****************************************************************************
 * FUNCTION
 *  category435_setup_editor_box
 * DESCRIPTION
 *  
 * PARAMETERS
 *  buffer              [?]         
 *  buffer_size         [IN]        
 *  input_type          [IN]        
 *  history_buffer      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
static void category435_setup_editor_box(U8 *buffer, S32 buffer_size, S16 input_type, U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 inputbox_x = CAT435_POPUP_X + CAT435_POPUP_H_GAP;
    S32 inputbox_y = CAT435_POPUP_Y + CAT435_POPUP_TITLE_HEIGHT;
    S32 inputbox_width = CAT435_POPUP_WIDTH - CAT435_POPUP_H_GAP * 2;
    S32 inputbox_height = CAT435_POPUP_HEIGHT - CAT435_POPUP_TITLE_HEIGHT - CAT435_POPUP_BOTTOM_GAP;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
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

    register_hide_multitap(category435_hide_multitap);

    /* Workaround. we need to set following variables */
    MMI_multiline_inputbox_width = inputbox_width;
    MMI_multiline_inputbox_height = inputbox_height;

    /* HACK. Arabic editor does not handle UI_MULTI_LINE_INPUT_BOX_AUTO_DISABLE_SCROLLBAR well */
    wgui_setup_inputbox_ext(
        inputbox_x,
        inputbox_y,
        inputbox_width,
        inputbox_height,
        buffer,
        buffer_size,
        MMI_CATEGORY435_ID,
        get_string(CAT435_CTX->editor_right_softkey),
        get_image(CAT435_CTX->editor_right_softkey_icon),
        input_type,
        history_buffer,
        1,
        r2lMMIFlag ? 0 : UI_MULTI_LINE_INPUT_BOX_AUTO_DISABLE_SCROLLBAR,
        0,
    #ifdef __MMI_T9__  /* T9 doesn't support icon in editor */
        0,
    #else
        (S16) (CAT435_CTX->icon_height + 1),
    #endif
        0,
        0,
    #ifdef __MMI_T9__
        NULL
    #else
        CAT435_CTX->icon_callback
    #endif
        );

    /* Resize editor height */
#if defined(__MMI_FULL_SCREEN_EDITOR__)
    inputbox_height =
        CAT435_POPUP_HEIGHT - CAT435_POPUP_TITLE_HEIGHT - CAT435_POPUP_BOTTOM_GAP -
        wgui_inputbox_information_bar_height - 1;
#else /* defined(__MMI_FULL_SCREEN_EDITOR__) */ 
    inputbox_height =
        CAT435_POPUP_HEIGHT - CAT435_POPUP_TITLE_HEIGHT - CAT435_POPUP_BOTTOM_GAP - MMI_multitap_height -
        wgui_inputbox_information_bar_height - 1;
#endif /* defined(__MMI_FULL_SCREEN_EDITOR__) */ 
    resize_multiline_inputbox(inputbox_width, inputbox_height);
    show_multiline_inputbox_no_draw();  /* recompute cursor position */

    /* Override editor style */
    MMI_multiline_inputbox.normal_filler = (UI_filled_area*)&g_mmi_gui_cat435_editor_filler;

#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    MMI_multiline_inputbox.flags |= UI_MULTI_LINE_INPUT_BOX_TRANSPARENT_BACKGROUND;
#endif 

    /* Key handler (category435_reset_editor_box reset wgui_inputbox_RSK_function) */
    wgui_inputbox_RSK_function = CAT435_CTX->editor_RSK_func;
    SetLeftSoftkeyFunction(CAT435_CTX->editor_LSK_func, KEY_EVENT_UP);

    /* Virtual keyboard */
#ifdef __MMI_TOUCH_SCREEN__
    dm_register_vkpad_callback(cat435_virtual_keypad_callback);
#endif 

    /* For draw manager, wgui_setup_inputbox_ext and wgui_close_inputbox should be paired. */
    MMI_DBG_ASSERT(!CAT435_CTX->editor_created);
    CAT435_CTX->editor_created = 1;
}


/*****************************************************************************
 * FUNCTION
 *  category435_reset_editor_box
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category435_reset_editor_box(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    /* Virtual keyboard */
#ifdef __MMI_TOUCH_SCREEN__
    dm_register_vkpad_callback(NULL);
#endif 
    wgui_close_inputbox();
    reset_multitaps();
    gui_unlock_double_buffer();

    CAT435_CTX->editor_created = 0;
}

/* We need to sync with T9 internal state */


/*****************************************************************************
 * FUNCTION
 *  category435_sync_with_T9
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category435_sync_with_T9(void)
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
             
    #if defined(__MMI_T9_INDONESIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_INDONESIAN:
        case INPUT_TYPE_SMART_LOWERCASE_INDONESIAN:
    #endif /* defined(__MMI_T9_INDONESIAN__) */ 
             
             
    #if defined(__MMI_T9_PORTUGUESE__)
        case INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE:
        case INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
    #endif /* defined(__MMI_T9_PORTUGUESE__) */ 
             
             

    #if defined(__MMI_T9_VIETNAMESE__)
        case INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
        case INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
    #endif /* defined(__MMI_T9_VIETNAMESE__) */ 

             
             
    #if defined(__MMI_T9_CZECH__)
        case INPUT_TYPE_SMART_UPPERCASE_CZECH:
        case INPUT_TYPE_SMART_LOWERCASE_CZECH:
    #endif /* defined(__MMI_T9_CZECH__) */ 
             
             
    #if defined(__MMI_T9_TURKISH__)
        case INPUT_TYPE_SMART_UPPERCASE_TURKISH:
        case INPUT_TYPE_SMART_LOWERCASE_TURKISH:
    #endif /* defined(__MMI_T9_TURKISH__) */ 
             
             
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
  
    #if defined(__MMI_T9_GREEK__)
        case INPUT_TYPE_SMART_UPPERCASE_GREEK:
        case INPUT_TYPE_SMART_LOWERCASE_GREEK:
    #endif /* defined(__MMI_T9_GREEK__) */ 
    #if defined(__MMI_T9_HEBREW__)
        case INPUT_TYPE_SMART_HEBREW:
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
 
  
    #if defined(__MMI_T9_SLOVENIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_SLOVENIAN:
        case INPUT_TYPE_SMART_LOWERCASE_SLOVENIAN:
    #endif /* defined(__MMI_T9_SLOVENIAN__) */ 
    #if defined(__MMI_T9_SWEDISH__)
        case INPUT_TYPE_SMART_UPPERCASE_SWEDISH:
        case INPUT_TYPE_SMART_LOWERCASE_SWEDISH:
    #endif /* defined(__MMI_T9_SWEDISH__) */ 
 
  
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
 * FUNCTION
 *  category435_draw_popup_background
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category435_draw_popup_background(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    extern UI_filled_area wgui_pop_up_dialog_background;
    S32 sw, sh; /* string width/height */
    S32 sx, sy;
    UI_filled_area f;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_push_clip();
    gui_reset_clip();

    gui_greyscale_rectangle(
        0,
        MMI_HORIZONTAL_TAB_BAR_Y + MMI_HORIZONTAL_TAB_BAR_TAB_AREA_HEIGHT,
        MMI_content_width - 1,
        CAT435_POPUP_Y + 15,    /* the dialog background has round corner */
        CAT435_BACKGROUND_WHITE_VALUE,
        MMI_BG_GREYSCALE_BLACK_VALUE);

    f = wgui_pop_up_dialog_background;
    f.c = g_mmi_gui_cat435_popup_color;
    f.border_color = current_multitap_input_theme->normal_text_color;   /* Tricky. use multitap text color as border color */
    wgui_draw_pop_up_screen_background_style2(
        CAT435_POPUP_X,
        CAT435_POPUP_Y,
        CAT435_POPUP_X + CAT435_POPUP_WIDTH - 1,
        CAT435_POPUP_Y + CAT435_POPUP_HEIGHT - 1,
        (UI_filled_area*)&f,
        MMI_TRUE,
        MMI_FALSE,
        MMI_FALSE,
        MMI_FALSE);

    gui_set_font(current_MMI_theme->window_title_font);
    gui_set_text_color(*current_MMI_theme->title_text_color);
#ifdef CAT435_POPUP_WITH_BORDER_TITLE
    gui_set_text_border_color(*current_MMI_theme->title_text_border_color);
#endif 
    Get_StringWidthHeight((U8*) CAT435_CTX->popup_title, &sw, &sh);

    if (sw > CAT435_POPUP_WIDTH)
    {
        sx = 0;
    }
    else
    {
        sx = (CAT435_POPUP_WIDTH - sw) >> 1;
    }

    if (sh > CAT435_POPUP_TITLE_HEIGHT)
    {
        sy = 0;
    }
    else
    {
        sy = (CAT435_POPUP_TITLE_HEIGHT - sh) >> 1;
    }

#ifdef CAT435_POPUP_WITH_BORDER_TITLE
    if (r2lMMIFlag)
    {
        gui_print_truncated_borderd_text(
            sx + CAT435_POPUP_X + sw - 1,
            sy + CAT435_POPUP_Y,
            CAT435_POPUP_WIDTH - sx,
            CAT435_CTX->popup_title);
    }
    else
    {
        gui_print_truncated_borderd_text(
            sx + CAT435_POPUP_X,
            sy + CAT435_POPUP_Y,
            CAT435_POPUP_WIDTH - sx,
            CAT435_CTX->popup_title);
    }
#else /* CAT435_POPUP_WITH_BORDER_TITLE */ 
    if (r2lMMIFlag)
    {
        gui_print_truncated_text(
            sx + CAT435_POPUP_X + sw - 1,
            sy + CAT435_POPUP_Y,
            CAT435_POPUP_WIDTH - sx,
            CAT435_CTX->popup_title);
    }
    else
    {
        gui_print_truncated_text(
            sx + CAT435_POPUP_X,
            sy + CAT435_POPUP_Y,
            CAT435_POPUP_WIDTH - sx,
            CAT435_CTX->popup_title);
    }
#endif /* CAT435_POPUP_WITH_BORDER_TITLE */ 

    gui_pop_clip();
}

/* Content area between title and softkey */


/*****************************************************************************
 * FUNCTION
 *  category435_draw_content_area
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void category435_draw_content_area(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Show background log area */
#ifdef CAT435_SHOW_LOG_BOX_WITH_POPUP
    gui_show_multi_line_input_box(&CAT435_CTX->log_box);
#else /* CAT435_SHOW_LOG_BOX_WITH_POPUP */ 
    if (!CAT435_CTX->show_popup)
    {
        gui_show_multi_line_input_box(&CAT435_CTX->log_box);
    }
#endif /* CAT435_SHOW_LOG_BOX_WITH_POPUP */ 

    /* Show editor in popup area */
    if (CAT435_CTX->show_popup)
    {
        category435_draw_popup_background();
        wgui_show_inputbox();
    }

    if (RedrawSpellingOrCandidateBoxesFunction)
    {
        RedrawSpellingOrCandidateBoxesFunction();
    }
}

#if defined(__MMI_TOUCH_SCREEN__)


/*****************************************************************************
 * FUNCTION
 *  category435_control_area_pen_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point           [IN]        
 *  pen_event       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category435_control_area_pen_handler(mmi_pen_point_struct point, mmi_pen_event_type_enum pen_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret;
    gui_multi_line_input_box_pen_enum logbox_event;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!CAT435_CTX)
    {
        MMI_DBG_ASSERT(0);
        return MMI_FALSE;
    }

    if (!CAT435_CTX->show_popup)
    {
        ret = gui_multi_line_input_box_translate_pen_event(
                &CAT435_CTX->log_box,
                pen_event,
                point.x,
                point.y,
                &logbox_event);
    }
    else
    {
        ret = MMI_FALSE;
    }

    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  category435_control_area_pen_down_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category435_control_area_pen_down_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return category435_control_area_pen_handler(point, MMI_PEN_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  category435_control_area_pen_move_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category435_control_area_pen_move_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return category435_control_area_pen_handler(point, MMI_PEN_EVENT_MOVE);
}


/*****************************************************************************
 * FUNCTION
 *  category435_control_area_pen_longtap_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category435_control_area_pen_longtap_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return category435_control_area_pen_handler(point, MMI_PEN_EVENT_LONG_TAP);
}


/*****************************************************************************
 * FUNCTION
 *  category435_control_area_pen_repeat_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category435_control_area_pen_repeat_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return category435_control_area_pen_handler(point, MMI_PEN_EVENT_REPEAT);
}


/*****************************************************************************
 * FUNCTION
 *  category435_control_area_pen_up_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category435_control_area_pen_up_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return category435_control_area_pen_handler(point, MMI_PEN_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  category435_control_area_pen_abort_handler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  point       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static MMI_BOOL category435_control_area_pen_abort_handler(mmi_pen_point_struct point)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return category435_control_area_pen_handler(point, MMI_PEN_EVENT_ABORT);
}

#endif /* defined(__MMI_TOUCH_SCREEN__) */ 

/***************************************************************************** 
* Global Variable
*****************************************************************************/

/***************************************************************************** 
* Global Function
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  ExitCategory420Screen
 * DESCRIPTION
 *  Exit the category420 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory420Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    category420_free_image_cache();
    reset_softkeys();
    reset_multitaps();
    reset_multiline_inputbox();
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;

    OslMfree(g_mmi_gui_cat420_context_p);
    g_mmi_gui_cat420_context_p = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory420Screen
 * DESCRIPTION
 *  Displays the category420 screen
 *  
 *  The pointer of name, mood_string, status_string are used directly  in category 420.
 *  They should not be released before category 420 exits.
 *  photo_filename can be released immediately after ShowCategory420Screen() returns.
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  name                    [IN]        User Name
 *  photo_filename          [IN]        File name of photo
 *  alternative_photo       [IN]        Default photo if 'photo_filename' is NULL
 *  mood_caption            [IN]        String ID to "Mood" literally
 *  mood_string             [IN]        Text indicating mood
 *  mood_icon               [IN]        Image indicating mood
 *  status_caption          [IN]        String ID to "Status" literally
 *  status_string           [IN]        Text indicating mood
 *  status_icon             [IN]        Image indicating mood
 *  buffer                  [IN]        Buffer the input box should use.
 *  buffer_size             [IN]        Size of the buffer.
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory420Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        UI_string_type name,
        S8 *photo_filename,
        U16 alternative_photo,
        U16 mood_caption,
        UI_string_type mood_string,
        PU8 mood_icon,
        U16 status_caption,
        UI_string_type status_string,
        PU8 status_icon,
        U8 *buffer,
        S32 buffer_size,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* Category 420 is a read-only multi-line inputbox with decoration */

    S32 l;
    S16 input_type = 0;
    S32 img_width, img_height;
    S32 use_alternative_image = 0;
    color_t tc;
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_gui_cat420_context_p = OslMalloc(sizeof(gui_cat420_context_struct));
    CAT420_CTX->name = name;
    CAT420_CTX->mood_caption = mood_caption;
    CAT420_CTX->mood_string = mood_string;
    CAT420_CTX->mood_icon = mood_icon;
    CAT420_CTX->status_caption = status_caption;
    CAT420_CTX->status_string = status_string;
    CAT420_CTX->status_icon = status_icon;

    /* Compute image dimension for rendering layout */
    if (!photo_filename || gdi_image_get_dimension_file(photo_filename, &img_width, &img_height) != GDI_IMAGE_SUCCEED)
    {
        if (!alternative_photo ||
            gdi_image_get_dimension((U8*) GetImage(alternative_photo), &img_width, &img_height) != GDI_IMAGE_SUCCEED)
        {
            CAT420_CTX->image_ori_width = 0;
            CAT420_CTX->image_ori_height = 0;
            CAT420_CTX->display_image = MMI_FALSE;
        }
        else
        {
            CAT420_CTX->image_ori_width = (S16) img_width;
            CAT420_CTX->image_ori_height = (S16) img_height;
            CAT420_CTX->display_image = MMI_TRUE;
            use_alternative_image = 1;
        }
    }
    else
    {
        CAT420_CTX->image_ori_width = (S16) img_width;
        CAT420_CTX->image_ori_height = (S16) img_height;
        CAT420_CTX->display_image = MMI_TRUE;
        use_alternative_image = 0;
    }

    category420_compute_layout();
    category420_cache_image(photo_filename, (U8*) GetImage(alternative_photo), use_alternative_image);

    gui_lock_double_buffer();

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    clear_category_screen_key_handlers();
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();

    l = gui_strlen((UI_string_type) buffer);
    create_multiline_inputbox_set_buffer((UI_string_type) buffer, buffer_size, l, 0);
    register_multiline_inputbox_viewer_keys();
    move_multiline_inputbox(MMI_content_x, MMI_content_y);
    resize_multiline_inputbox(MMI_multiline_inputbox.width, MMI_content_height);
    MMI_multiline_inputbox.flags |= (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW |
                                     UI_MULTI_LINE_INPUT_BOX_VIEW_MODE |
                                     UI_MULTI_LINE_INPUT_BOX_AUTO_DISABLE_SCROLLBAR);
    MMI_multiline_inputbox.header_callback = category420_header_callback;
    MMI_multiline_inputbox.header_height = CAT420_CTX->editor_header_height;
    MMI_multiline_inputbox.footer_height = CAT420_EDITOR_BOTTOM_PADDING;
    /* Better visual effect */
    MMI_multiline_inputbox.min_text_height = MMI_content_height - CAT420_EDITOR_BOTTOM_PADDING;
    MMI_multiline_inputbox.text_filler = &g_mmi_gui_cat420_text_filler;
    tc.r = CAT420_TEXT_COLOR_R;
    tc.g = CAT420_TEXT_COLOR_G;
    tc.b = CAT420_TEXT_COLOR_B;
    tc.alpha = 100;
    MMI_multiline_inputbox.normal_text_color = tc;
    MMI_multiline_inputbox.text_x = CAT420_EDITOR_LEFT_BORDER_PADDING;
    MMI_multiline_inputbox.text_x2_gap_width = CAT420_EDITOR_RIGHT_BORDER_PADDING;
    /* resize_multiline_inputbox() recomputes MMI_multiline_inputbox.edit_width by 'text_x' and 'text_x2_gap' */
    resize_multiline_inputbox(MMI_multiline_inputbox.width, MMI_multiline_inputbox.height);

    show_multiline_inputbox_no_draw();

    set_multiline_inputbox_category_history(MMI_CATEGORY420_ID, history_buffer, &input_type);
    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategory420Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY420_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}

#ifdef __MMI_UI_TAB_PANE__
/*****************************************************************************
 * FUNCTION
 *  RedrawCategory425Screen
 * DESCRIPTION
 *  Redraw the category425 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void RedrawCategory425Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();
    show_title_status_icon();
    show_asyncdynamic_list();
    if (CAT425_CTX->num_tab)
    {
        wgui_show_horizontal_tab_bar();
    }
    show_softkey_background();
    show_left_softkey();
    show_right_softkey();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory425Screen
 * DESCRIPTION
 *  Exit the category425 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory425Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearHighlightHandler();
    reset_softkeys();
    reset_menu_shortcut_handler();
    if (CAT425_CTX->num_tab)
    {
        wgui_close_horizontal_tab_bar();
    }
    reset_asyncdynamic_list();
    reset_pop_up_descriptions();

    category425_reset_context();
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory425HistorySize
 * DESCRIPTION
 *  Gets the size of the history buffer for category425 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *  Returns
 *****************************************************************************/
S32 GetCategory425HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (sizeof(list_menu_category_history));
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory425History
 * DESCRIPTION
 *  Get history for category425 screen
 * PARAMETERS
 *  history_buffer      [IN]        Is the buffer into which the history data is stored (pre-allocated)
 * RETURNS
 *  Pointer to the history buffer
 *****************************************************************************/
U8 *GetCategory425History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    get_list_menu_category_history(MMI_CATEGORY425_ID, history_buffer);
    return history_buffer;
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory425Screen
 * DESCRIPTION
 *  Displays the category425 screen
 *  
 *  1. Each menu item has two text strings: name and category.
 *  2. Each menu item has three image icons: buddy icon, message icon, and block icon.
 *  An empty icon can be set as NULL in 'get_item_func'
 *  3. Hint display id not visual appealing in this category screen, and should be avoided.
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  n_tabs                  [IN]        Number of tabs
 *  highlighted_tab         [IN]        Highlighted tab
 *  tab_items               [IN]        Tab list
 *  number_of_items         [IN]        Number of menu items
 *  get_item_func           [IN]        Callback function to get menu text.
 *  get_hint_func           [IN]        (Deprecated) Callback function to get menu hint.
 *  sample_icon             [IN]        Sample icon to get image dimension of buddy icon
 *  sample_icon2            [IN]        Sample icon to get image dimension of message icon
 *  highlighted_item        [IN]        Default highlighted menu item
 *  history_buffer          [IN]        History buffer
 *  list_is_ready_p         [OUT]       Whether the dynamic list menu is successfully loaded.
 *  in(?)                   [IN]        The bottom-leftmost corner
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory425Screen(
        UI_string_type title,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        S8 n_tabs,
        S8 highlighted_tab,
        tab_bar_item_type *tab_items,
        S32 number_of_items,
        GetAsyncItemFuncPtr get_item_func,
        GetAsyncHintFuncPtr get_hint_func,
        PU8 sample_icon,
        PU8 sample_icon2,
        S32 highlighted_item,
        U8 *history_buffer,
        BOOL *list_is_ready_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    U8 h_flag;
    S32 icon_height, icon_width, icon2_height, icon2_width, text_width;
    S32 tmp_string_width;
    int i;
    S32 shortcut_width;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_asyncdynamic_list_error = 0;
    *list_is_ready_p = MMI_FALSE;
    g_mmi_gui_cat425_context_p = OslMalloc(sizeof(gui_cat425_context_struct));

    CAT425_CTX->get_item_func = get_item_func;
    CAT425_CTX->get_hint_func = get_hint_func;

    MMI_title_string = NULL;
    MMI_title_icon = NULL;

    gui_lock_double_buffer();

    clear_category_screen_key_handlers();

    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();

    /* Setup tab bar */
    CAT425_CTX->num_tab = n_tabs;
    if (n_tabs)
    {
        for (i = 0; i < n_tabs; i++)
        {
            MMI_tab_bar_items[i] = tab_items[i];
        }
        wgui_create_horizontal_tab_bar(TRUE, title, n_tabs, highlighted_tab, MMI_FALSE);

        CAT425_CTX->content_x = MMI_CONTENT_X_WITH_H_TAB;
        CAT425_CTX->content_y = MMI_CONTENT_Y_WITH_H_TAB;
        CAT425_CTX->content_width = MMI_CONTENT_WIDTH_WITH_H_TAB;
        CAT425_CTX->content_height = MMI_CONTENT_HEIGHT_WITH_H_TAB;
    }
    else
    {
        MMI_title_string = title;
        CAT425_CTX->content_x = MMI_content_x;
        CAT425_CTX->content_y = MMI_content_y;
        CAT425_CTX->content_width = MMI_content_width;
        CAT425_CTX->content_height = MMI_content_height;
    }

    /* Setup layout of menu */
    create_fixed_icontext_list_menuitems(2, 3);
    gui_measure_image(sample_icon, &icon_width, &icon_height);
    gui_measure_image(sample_icon2, &icon2_width, &icon2_height);
    if (icon_width == 0 || icon_height == 0)
    {
        icon_width = CAT425_DEFAULT_ICON_WIDTH;
        icon_height = CAT425_DEFAULT_ICON_HEIGHT;
    }
    if (icon_width == 0 || icon_height == 0)
    {
        icon2_width = CAT425_DEFAULT_ICON2_WIDTH;
        icon2_height = CAT425_DEFAULT_ICON2_HEIGHT;
    }

    tmp_string_width = gui_get_string_width((UI_string_type) L"[");
    text_width = UI_device_width - MMI_fixed_list_menu.vbar.width - CAT425_ICON_X -
        icon_width - CAT425_ICON_TEXT_GAP - CAT425_TEXT_SCROLLBAR_GAP;
    set_fixed_icontext_list_icon_coordinates(0, CAT425_ICON_X, CAT425_ICON_Y, icon_width, icon_height);
    if (r2lMMIFlag)
    {
        set_fixed_icontext_list_icon_coordinates(
            1,
            CAT425_ICON_X + icon_width - icon2_width,
            CAT425_ICON_Y + icon_height - icon2_height,
            icon2_width,
            icon2_height);
    }
    else
    {
        set_fixed_icontext_list_icon_coordinates(
            1,
            CAT425_ICON_X,
            CAT425_ICON_Y + icon_height - icon2_height,
            icon2_width,
            icon2_height);
    }
    set_fixed_icontext_list_icon_coordinates(2, CAT425_ICON_X, CAT425_ICON_Y, icon_width, icon_height);

    set_fixed_icontext_list_text_coordinates(
        0,
        CAT425_ICON_X + icon_width + CAT425_ICON_TEXT_GAP,
        CAT425_TEXT1_Y,
        text_width,
        CAT425_TEXT_HEIGHT);
    set_fixed_icontext_list_text_coordinates(
        1,
        CAT425_ICON_X + icon_width + CAT425_ICON_TEXT_GAP + tmp_string_width + CAT425_TEXT2_BRACKET_GAP,
        CAT425_TEXT2_Y,
        text_width - (tmp_string_width + CAT425_TEXT2_BRACKET_GAP) * 2,
        CAT425_TEXT_HEIGHT);

    CAT425_CTX->icon_border_x1 = CAT425_ICON_X;
    CAT425_CTX->icon_border_y1 = CAT425_ICON_Y;
    CAT425_CTX->icon_border_x2 = CAT425_ICON_X + icon_width - 1;
    CAT425_CTX->icon_border_y2 = CAT425_ICON_Y + icon_height - 1;
    CAT425_CTX->bracket_x = CAT425_ICON_X + icon_width + CAT425_ICON_TEXT_GAP;
    CAT425_CTX->bracket_y = CAT425_TEXT2_Y;

    /* Setup data of menu */
    associate_fixed_icontext_list_list();
    set_fixed_icontext_list_menuitems_post_display(category425_post_display_menuitem);
    MMI_fixed_icontext_list_menuitem.flags |= UI_MENUITEM_AUTO_CHOOSE_MARQUEE;
    resize_fixed_icontext_list_menuitems(0, n_tabs ? CAT425_MENUITEM_HEIGHT : CAT425_MENUITEM_HEIGHT_NO_TAB);   /* Tricky! do this after associate_fixed_icontext_list_list() */
    CAT425_CTX->backup_menu_flags = MMI_fixed_list_menu.flags;
    CAT425_CTX->backup_menu_filler = MMI_fixed_list_menu.normal_filler;

    /* Override style */
    // Disable background image 
    // MMI_fixed_list_menu.normal_filler = &g_mmi_gui_cat425_list_filler;
    category425_setup_text_color();
    MMI_fixed_icontext_list_menuitem.flags &=
        ~(UI_MENUITEM_CENTER_TEXT_X | UI_MENUITEM_CENTER_TEXT_Y | UI_MENUITEM_CENTER_ICON_X |
          UI_MENUITEM_CENTER_ICON_Y);

    MMI_current_menu_type = LIST_MENU;
    MMI_fixed_list_menu.n_items = number_of_items;
    if (!init_asyncdynamic_item_buffer(number_of_items, get_item_func, get_hint_func))
    {
        MMI_DBG_ASSERT(0);  /* not recoverable */
        category425_reset_context();
        gui_unlock_double_buffer();
        return;
    }
    register_asyncdynamic_list_keys();

    resize_fixed_list(CAT425_CTX->content_width, CAT425_CTX->content_height);
    move_fixed_list(CAT425_CTX->content_x, CAT425_CTX->content_y);
    register_fixed_list_highlight_handler(asyncdynamic_list_highlight_handler);
    h_flag = set_list_menu_category_history(MMI_CATEGORY425_ID, history_buffer);
    if (h_flag)
    {
        highlighted_item = MMI_fixed_list_menu.highlighted_item;
    }
    asyncdynamic_list_goto_item_no_redraw(highlighted_item);
    if (gui_asyncdynamic_list_error)
    {
        MMI_DBG_ASSERT(0);  /* not recoverable */
        category425_reset_context();
        gui_unlock_double_buffer();
        return;
    }
    set_asyncdynamic_pop_up_descriptions(highlighted_item);

    /* Setup (hidden) menu shortcut. */
    MMI_menu_shortcut_number = highlighted_item + 1;
    register_asyncdynamic_list_shortcut_handler();
    shortcut_width = set_menu_item_count(number_of_items) + 7;
    if (n_tabs)
    {
        resize_menu_shortcut_handler(shortcut_width, MMI_HORIZONTAL_TAB_BAR_HINT_AREA_HEIGHT);
        move_menu_shortcut_handler(
            MMI_HORIZONTAL_TAB_BAR_X + MMI_HORIZONTAL_TAB_BAR_WIDTH - MMI_menu_shortcut_box.width,
            MMI_HORIZONTAL_TAB_BAR_Y + MMI_HORIZONTAL_TAB_BAR_TAB_AREA_HEIGHT);
        register_hide_menu_shortcut(wgui_show_horizontal_tab_bar_hint_area);
    }
    else
    {
        resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
        move_menu_shortcut_handler(MMI_title_x + MMI_title_width - MMI_menu_shortcut_box.width, MMI_title_y);
        register_hide_menu_shortcut(gui_redraw_menu_shortcut);
    }

    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategory425Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    if (n_tabs)
    {
        dm_data.S32CatId = MMI_CATEGORY425_ID;
    }
    else
    {
        dm_data.S32CatId = MMI_CATEGORY425_NO_TAB_ID;
    }
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

    *list_is_ready_p = MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  RefreshCategory425Screen
 * DESCRIPTION
 *  Refresh the category425 screen
 *  (it will reload menu item data with 'get_item_func' passed in ShowCategory425Screen)
 *  
 *  Category 425 screen should be currently displayed.
 * PARAMETERS
 *  number_of_items         [IN]        The number of menu items.
 *  highlighted_item        [IN]        The highlighted item
 * RETURNS
 *  void
 *****************************************************************************/
void RefreshCategory425Screen(S32 number_of_items, S32 highlighted_item)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!g_mmi_gui_cat425_context_p)
    {
        MMI_DBG_ASSERT(0);
        return;
    }

    gui_asyncdynamic_list_error = 0;

    gui_lock_double_buffer();

    MMI_fixed_list_menu.n_items = number_of_items;
    init_asyncdynamic_item_buffer(number_of_items, CAT425_CTX->get_item_func, CAT425_CTX->get_hint_func);

    asyncdynamic_list_goto_item_no_redraw(highlighted_item);
    if (gui_asyncdynamic_list_error)
    {
        MMI_DBG_ASSERT(0);  /* not recoverable */
        category425_reset_context();
        gui_unlock_double_buffer();
        return;
    }

    set_asyncdynamic_pop_up_descriptions(highlighted_item);

    gui_unlock_double_buffer();

    RedrawCategoryFunction();
}


/*****************************************************************************
 * FUNCTION
 *  UpdateCategory425TabBar
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title               [IN]        
 *  n_tabs              [IN]        
 *  highlighted_tab     [IN]        
 *  tab_items           [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void UpdateCategory425TabBar(UI_string_type title, S8 n_tabs, S8 highlighted_tab, tab_bar_item_type *tab_items)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!CAT425_CTX || !CAT425_CTX->num_tab)
    {
        MMI_DBG_ASSERT(0);
        return;
    }

    if (title == NULL)
    {
        title = MMI_horizontal_tab_bar.hint;
    }

    gui_lock_double_buffer();

    wgui_close_horizontal_tab_bar();

    /* Setup horizontal tab bar */
    for (i = 0; i < n_tabs; i++)
    {
        MMI_tab_bar_items[i] = tab_items[i];
    }

    wgui_create_horizontal_tab_bar(TRUE, title, n_tabs, highlighted_tab, MMI_TRUE);

    gui_unlock_double_buffer();

    wgui_show_horizontal_tab_bar();
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory425TabSelectCallback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  select_callback     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory425TabSelectCallback(void (*select_callback) (int index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_DBG_ASSERT(CAT425_CTX && CAT425_CTX->num_tab);

    wgui_set_horizontal_tab_bar_select_callback(select_callback);
}


/*****************************************************************************
 * FUNCTION
 *  Category425BlinkTab
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tab     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category425BlinkTab(S8 tab)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_DBG_ASSERT(CAT425_CTX && CAT425_CTX->num_tab);

    wgui_horizontal_tab_bar_start_blinking(tab);
}


/*****************************************************************************
 * FUNCTION
 *  Category425UnBlinkTab
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tab     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category425UnBlinkTab(S8 tab)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_DBG_ASSERT(CAT425_CTX && CAT425_CTX->num_tab);

    wgui_horizontal_tab_bar_stop_blinking(tab);
}
#endif
#if (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_SMART_MESSAGE_MT__) || defined(__MMI_MYPICTURE__)


/*****************************************************************************
 * FUNCTION
 *  ShowCategoryNSM2Screen
 * DESCRIPTION
 *  Displays the category420 screen
 *  
 *  The pointer of name, mood_string, status_string are used directly  in category 420.
 *  They should not be released before category 420 exits.
 *  photo_filename can be released immediately after ShowCategory420Screen() returns.
 * PARAMETERS
 *  title                   [IN]        Title for the screen
 *  title_icon              [IN]        Icon shown with the title
 *  left_softkey            [IN]        Left softkey label
 *  left_softkey_icon       [IN]        Icon for the left softkey
 *  right_softkey           [IN]        Right softkey label
 *  right_softkey_icon      [IN]        Icon for the right softkey
 *  name                    [IN]        User Name
 *  photo_filename          [IN]        File name of photo
 *  alternative_photo       [IN]        Default photo if 'photo_filename' is NULL
 *  mood_caption            [IN]        String ID to "Mood" literally
 *  mood_string             [IN]        Text indicating mood
 *  mood_icon               [IN]        Image indicating mood
 *  status_caption          [IN]        String ID to "Status" literally
 *  status_string           [IN]        Text indicating mood
 *  status_icon             [IN]        Image indicating mood
 *  buffer                  [IN]        Buffer the input box should use.
 *  buffer_size             [IN]        Size of the buffer.
 *  history_buffer          [IN]        History buffer
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategoryNSM2Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        UI_string_type name,
        S8 *photo_filename,
        U16 alternative_photo,
        U16 mood_caption,
        UI_string_type mood_string,
        PU8 mood_icon,
        U16 status_caption,
        UI_string_type status_string,
        PU8 status_icon,
        U8 *buffer,
        S32 buffer_size,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 l;
    S16 input_type = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_gui_cat420_context_p = OslMalloc(sizeof(gui_cat420_context_struct));
    CAT420_CTX->name = name;
    CAT420_CTX->mood_caption = mood_caption;
    CAT420_CTX->mood_string = mood_string;
    CAT420_CTX->mood_icon = mood_icon;
    CAT420_CTX->status_caption = status_caption;
    CAT420_CTX->status_string = status_string;
    CAT420_CTX->status_icon = status_icon;

    CAT420_CTX->image_ori_width = 120;
    CAT420_CTX->image_ori_height = 30;
    CAT420_CTX->display_image = MMI_TRUE;

    category420_compute_layout();
    gui_lock_double_buffer();

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    clear_category_screen_key_handlers();
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();

    l = gui_strlen((UI_string_type) buffer);
    create_multiline_inputbox_set_buffer((UI_string_type) buffer, buffer_size, l, 0);
    register_multiline_inputbox_viewer_keys();

    MMI_multiline_inputbox.flags |= (UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW | UI_MULTI_LINE_INPUT_BOX_VIEW_MODE);
    MMI_multiline_inputbox.header_callback = mmi_nsm2_category275_draw_inbox_header;    /* To draw image */
    /* JP to fix NSM Picture image and chinese character overlapping */
    MMI_multiline_inputbox.header_height = MAX_NSM_PIC_HEIGHT;  /* CAT420_CTX->editor_header_height; */

    MMI_multiline_inputbox.min_text_height = 14;

    show_multiline_inputbox_no_draw();

    set_multiline_inputbox_category_history(MMI_CATEGORY420_ID, history_buffer, &input_type);
    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategoryNSM2Screen;
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY420_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  ExitCategoryNSM2Screen
 * DESCRIPTION
 *  Exit the categoryNS screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategoryNSM2Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    // category420_free_image_cache();  /*JP*/
    reset_softkeys();
    reset_multitaps();
    reset_multiline_inputbox();
    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;

    OslMfree(g_mmi_gui_cat420_context_p);
    g_mmi_gui_cat420_context_p = NULL;
}

#endif /* (defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)) || defined(__MMI_SMART_MESSAGE_MT__) || defined(__MMI_MYPICTURE__) */ 

/* 
 * Category 430
 */


/*****************************************************************************
 * FUNCTION
 *  ExitCategory430Screen
 * DESCRIPTION
 *  Exits the category430 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory430Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_softkeys();
    wgui_close_inputbox();

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;

    if (CAT430_CTX->allocated_color_buffer)
    {
        OslMfree(CAT430_CTX->allocated_color_buffer);
    }

    OslMfree(g_mmi_gui_cat430_context_p);
    g_mmi_gui_cat430_context_p = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory430HistorySize
 * DESCRIPTION
 *  Gets the size of the history buffer for category430 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  size in U8 of the history buffer
 *****************************************************************************/
S32 GetCategory430HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Use the four bytes to check if popup exists */
    return sizeof(gui_cat430_history_header_struct);
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory430History
 * DESCRIPTION
 *  Gets the history buffer for category430 screen
 * PARAMETERS
 *  history_buffer      [IN]        Is the buffer into which the history data is stored (pre-allocated)
 * RETURNS
 *  Pointer to the history buffer
 *****************************************************************************/
U8 *GetCategory430History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Use the four bytes to check if popup exists */
    MMI_ASSERT(g_mmi_gui_cat430_context_p);

    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory430Screen
 * DESCRIPTION
 *  Displays the category430 screen
 * PARAMETERS
 *  title                       [IN]        
 *  title_icon                  [IN]        
 *  left_softkey                [IN]        
 *  left_softkey_icon           [IN]        
 *  right_softkey               [IN]        
 *  right_softkey_icon          [IN]        
 *  log_buffer1                 [?]         
 *  log_buffer_size1            [IN]        
 *  log_buffer2                 [?]         
 *  log_buffer_size2            [IN]        
 *  show_sending_buffer         [IN]        
 *  max_sending_buffer_size     [IN]        
 *  sending_buffer              [?]         
 *  sent_count                  [IN]        
 *  name1                       [?]         
 *  name2                       [?]         
 *  history_buffer              [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory430Screen(
        U16 title,
        U16 title_icon,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U8 *log_buffer1,
        S16 log_buffer_size1,
        U8 *log_buffer2,
        S16 log_buffer_size2,
        MMI_BOOL show_sending_buffer,
        S16 max_sending_buffer_size,
        U8 *sending_buffer,
        S16 sent_count,
        U8 *name1,
        U8 *name2,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    dm_data_struct dm_data;
    S32 font_height, log_box_height;

    /*----------------------------------------------------------------*/
    /* Global Variables                                                */
    /*----------------------------------------------------------------*/

    g_mmi_gui_cat430_context_p = OslMalloc(sizeof(gui_cat430_context_struct));
    CAT430_CTX->name1 = name1;
    CAT430_CTX->name2 = name2;
    CAT430_CTX->log_buffer1 = log_buffer1;
    CAT430_CTX->log_buffer_size1 = log_buffer_size1;
    CAT430_CTX->log_buffer2 = log_buffer2;
    CAT430_CTX->log_buffer_size2 = log_buffer_size2;

    CAT430_CTX->show_sending_buffer = show_sending_buffer;
    CAT430_CTX->max_sending_buffer_size = max_sending_buffer_size;

    CAT430_CTX->sending_buffer = sending_buffer;
    CAT430_CTX->sent_count = sent_count;

    CAT430_CTX->allocated_color_buffer_size = (max_sending_buffer_size + CAT430_MARKER_COUNT + 1) * 2;
    CAT430_CTX->allocated_color_buffer = OslMalloc(CAT430_CTX->allocated_color_buffer_size);

    /*----------------------------------------------------------------*/
    /* Compute Layout                                                  */
    /*----------------------------------------------------------------*/
    font_height = Get_CharHeightOfAllLang(CAT430_NAME_FONT_ENUM);
#ifndef CAT430_NAME_IN_LOG
    log_box_height = MMI_content_height - (font_height << 1) - (CAT430_VERTICAL_GAP * 5);
    log_box_height >>= 1;
    MMI_DBG_ASSERT(log_box_height > 0);
#else /* CAT430_NAME_IN_LOG */ 
    CAT430_CTX->log_box_header_height = font_height + CAT430_VERTICAL_GAP;
    log_box_height = MMI_content_height - (CAT430_VERTICAL_GAP * 3);
    log_box_height >>= 1;
    MMI_DBG_ASSERT(log_box_height > 0);
#endif /* CAT430_NAME_IN_LOG */ 

#ifndef CAT430_NAME_IN_LOG
    CAT430_CTX->name1_x = CAT430_HORIZONTAL_GAP;
    CAT430_CTX->name1_y = MMI_content_y + CAT430_VERTICAL_GAP;
    CAT430_CTX->log_box1_y = CAT430_CTX->name1_y + font_height + CAT430_VERTICAL_GAP;
#else /* CAT430_NAME_IN_LOG */ 
    CAT430_CTX->log_box1_y = MMI_content_y + CAT430_VERTICAL_GAP;
#endif /* CAT430_NAME_IN_LOG */ 
    CAT430_CTX->log_box1_x = CAT430_HORIZONTAL_GAP;
    CAT430_CTX->log_box1_width = MMI_content_width - CAT430_HORIZONTAL_GAP * 2;
    CAT430_CTX->log_box1_height = log_box_height;
#ifndef CAT430_NAME_IN_LOG
    CAT430_CTX->name2_x = CAT430_HORIZONTAL_GAP;
    CAT430_CTX->name2_y = CAT430_CTX->log_box1_y + CAT430_CTX->log_box1_height + CAT430_VERTICAL_GAP;
    CAT430_CTX->log_box2_y = CAT430_CTX->name2_y + font_height + CAT430_VERTICAL_GAP;
#else /* CAT430_NAME_IN_LOG */ 
    CAT430_CTX->log_box2_y = CAT430_CTX->log_box1_y + CAT430_CTX->log_box1_height + CAT430_VERTICAL_GAP;
#endif /* CAT430_NAME_IN_LOG */ 
    CAT430_CTX->log_box2_x = CAT430_HORIZONTAL_GAP;
    CAT430_CTX->log_box2_width = CAT430_CTX->log_box1_width;
    CAT430_CTX->log_box2_height = log_box_height;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();

    MMI_title_string = (UI_string_type) get_string(title);
    MMI_title_icon = (PU8) get_image(title_icon);

    category430_setup_sending_box();
    category430_setup_log_box1();
    category430_setup_log_box2();

    SetKeyHandler(category430_previous_line, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(category430_next_line, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(category430_previous_line, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(category430_next_line, KEY_VOL_DOWN, KEY_EVENT_DOWN);

    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategory430Screen;
    dm_register_category_controlled_callback(category430_draw_controlled_area);
    dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY430_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_register_category_screen_control_area_pen_handlers(
        category430_control_area_pen_down_handler,
        MMI_PEN_EVENT_DOWN);
    wgui_register_category_screen_control_area_pen_handlers(category430_control_area_pen_up_handler, MMI_PEN_EVENT_UP);
    wgui_register_category_screen_control_area_pen_handlers(
        category430_control_area_pen_move_handler,
        MMI_PEN_EVENT_MOVE);
    wgui_register_category_screen_control_area_pen_handlers(
        category430_control_area_pen_repeat_handler,
        MMI_PEN_EVENT_REPEAT);
    wgui_register_category_screen_control_area_pen_handlers(
        category430_control_area_pen_longtap_handler,
        MMI_PEN_EVENT_LONG_TAP);
    wgui_register_category_screen_control_area_pen_handlers(
        category430_control_area_pen_abort_handler,
        MMI_PEN_EVENT_ABORT);
#endif /* __MMI_TOUCH_SCREEN__ */ 
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}


/*****************************************************************************
 * FUNCTION
 *  UpdateCategory430LogBox
 * DESCRIPTION
 *  
 * PARAMETERS
 *  update_log1     [IN]        
 *  update_log2     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UpdateCategory430LogBox(MMI_BOOL update_log1, MMI_BOOL update_log2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(g_mmi_gui_cat430_context_p);

    if (update_log1)
    {
        category430_setup_log_box1();
    }
    if (update_log2)
    {
        category430_setup_log_box2();
    }
    RedrawCategoryFunction();

}


/*****************************************************************************
 * FUNCTION
 *  ConfigureCategory430SendingBox
 * DESCRIPTION
 *  
 * PARAMETERS
 *  show_sending_buffer     [IN]        
 *  sent_count              [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ConfigureCategory430SendingBox(MMI_BOOL show_sending_buffer, S16 sent_count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(g_mmi_gui_cat430_context_p);

    CAT430_CTX->show_sending_buffer = show_sending_buffer;
    CAT430_CTX->sent_count = sent_count;

    category430_setup_sending_box();
    /* Setup logbox2 because 'log_box2_footer_height' might be changed */
    category430_setup_log_box2();
    RedrawCategoryFunction();
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory430SendingBoxCharacterCount
 * DESCRIPTION
 *  
 * PARAMETERS
 *  sent_count      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory430SendingBoxCharacterCount(S16 sent_count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CAT430_CTX->sent_count = sent_count;
    RedrawCategoryFunction();
}


/*****************************************************************************
 * FUNCTION
 *  UpdateCategory430LSK
 * DESCRIPTION
 *  
 * PARAMETERS
 *  left_softkey            [IN]        
 *  left_softkey_icon       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UpdateCategory430LSK(U16 left_softkey, U16 left_softkey_icon)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();

    change_left_softkey(left_softkey, left_softkey_icon);
    /* setup key handler again if previous softkey is empty */
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();

    show_softkey_background();
    show_left_softkey();
    show_right_softkey();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
 * FUNCTION
 *  UpdateCategory430RSK
 * DESCRIPTION
 *  
 * PARAMETERS
 *  right_softkey           [IN]        
 *  right_softkey_icon      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UpdateCategory430RSK(U16 right_softkey, U16 right_softkey_icon)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();

    change_right_softkey(right_softkey, right_softkey_icon);
    /* setup key handler again if previous softkey is empty */
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();

    show_softkey_background();
    show_left_softkey();
    show_right_softkey();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}

/* 
 * Category 435
 */


/*****************************************************************************
 * FUNCTION
 *  category435_draw_controlled_area
 * DESCRIPTION
 *  Category-controlled area draw callback for category435 screen
 * PARAMETERS
 *  coordinate      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void category435_draw_controlled_area(dm_coordinates *coordinate)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(coordinate);
    category435_draw_content_area();
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory435EditorRSKFunction
 * DESCRIPTION
 *  Set the right softkey function for editor popup in  category435 screen
 * PARAMETERS
 *  f       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory435EditorRSKFunction(void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CAT435_CTX->editor_RSK_func = f;
    wgui_inputbox_RSK_function = f;
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory435EditorLSKFunction
 * DESCRIPTION
 *  Set the left softkey function for editor popup in  category435 screen
 * PARAMETERS
 *  f       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory435EditorLSKFunction(void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* ShowCategory435Screen should be invoked first */
    MMI_ASSERT(CAT435_CTX);

    CAT435_CTX->editor_LSK_func = f;
    if (CAT435_CTX->show_popup)
    {
        SetLeftSoftkeyFunction(f, KEY_EVENT_UP);
    }
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory435RSKFunction
 * DESCRIPTION
 *  Set the left softkey function category435 screen without popup
 * PARAMETERS
 *  f       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory435RSKFunction(void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* ShowCategory435Screen should be invoked first */
    MMI_ASSERT(CAT435_CTX);

    CAT435_CTX->RSK_func = f;
    if (!CAT435_CTX->show_popup)
    {
        SetRightSoftkeyFunction(f, KEY_EVENT_UP);
    }
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory435LSKFunction
 * DESCRIPTION
 *  Set the left softkey function category435 screen without popup
 * PARAMETERS
 *  f       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory435LSKFunction(void (*f) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* ShowCategory435Screen should be invoked first */
    MMI_ASSERT(CAT435_CTX);

    CAT435_CTX->LSK_func = f;
    if (!CAT435_CTX->show_popup)
    {
        SetLeftSoftkeyFunction(f, KEY_EVENT_UP);
    }
}


/*****************************************************************************
 * FUNCTION
 *  ExitCategory435Screen
 * DESCRIPTION
 *  Exits the category435 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitCategory435Screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    reset_softkeys();
    wgui_close_horizontal_tab_bar();
    set_MMI_current_input_type();
    if (CAT435_CTX->show_popup)
    {
        category435_reset_editor_box();
    }

#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    dm_set_scr_bg_image_no_draw(
        editor_scr_bg_ID,
        editor_scr_bg_filename,
        editor_scr_bg_x,
        editor_scr_bg_y,
        editor_scr_bg_opacity);
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;

    /* Restore context */
    MMI_multiline_inputbox_width = CAT435_CTX->backup_MMI_editor_width;
    MMI_multiline_inputbox_height = CAT435_CTX->backup_MMI_editor_height;

    current_information_bar_theme->background_color = CAT435_CTX->backup_information_bar_color;

    current_multitap_input_theme->f = CAT435_CTX->backup_multitap_filler;
    set_MMI_multitap_theme();

    OslMfree(g_mmi_gui_cat435_context_p);
    g_mmi_gui_cat435_context_p = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory435HistorySize
 * DESCRIPTION
 *  Gets the size of the history buffer for category435 screen
 * PARAMETERS
 *  void
 * RETURNS
 *  size in U8 of the history buffer
 *****************************************************************************/
S32 GetCategory435HistorySize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Use the four bytes to check if popup exists */
    return sizeof(gui_cat435_history_header_struct) + sizeof(multiline_inputbox_category_history);
}


/*****************************************************************************
 * FUNCTION
 *  GetCategory435History
 * DESCRIPTION
 *  Gets the history buffer for category435 screen
 * PARAMETERS
 *  history_buffer      [IN]        Is the buffer into which the history data is stored (pre-allocated)
 * RETURNS
 *  Pointer to the history buffer
 *****************************************************************************/
U8 *GetCategory435History(U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* Use the four bytes to check if popup exists */
    gui_cat435_history_header_struct *h = (gui_cat435_history_header_struct*) history_buffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    h->show_popup = (U8) CAT435_CTX->show_popup;
    if (CAT435_CTX->show_popup)
    {
        /* Get editor history only if editor is active */
        get_multiline_inputbox_category_history(
            MMI_CATEGORY435_ID,
            history_buffer + sizeof(gui_cat435_history_header_struct),
            MMI_current_input_type);
    }
    return (history_buffer);
}


/*****************************************************************************
 * FUNCTION
 *  UpdateCategory435LogText
 * DESCRIPTION
 *  Update the log text in category 435
 * PARAMETERS
 *  log_buffer          [IN]        
 *  log_buffer_size     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UpdateCategory435LogText(U8 *log_buffer, S32 log_buffer_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* ShowCategory435Screen should be invoked first */
    MMI_ASSERT(CAT435_CTX);

    gui_lock_double_buffer();
    category435_setup_log_box(log_buffer, log_buffer_size);
    gui_unlock_double_buffer();

    /* We do not update the background if popup is displayed */
    if (!CAT435_CTX->show_popup)
    {
        RedrawCategoryFunction();
    }
}


/*****************************************************************************
 * FUNCTION
 *  UpdateCategory435LogAndEditorText
 * DESCRIPTION
 *  Update the log and editor text in category 435
 * PARAMETERS
 *  buffer              [IN]        
 *  buffer_size         [IN]        
 *  log_buffer          [IN]        
 *  log_buffer_size     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void UpdateCategory435LogAndEditorText(U8 *buffer, S32 buffer_size, U8 *log_buffer, S32 log_buffer_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* ShowCategory435Screen should be invoked first */
    MMI_ASSERT(CAT435_CTX);

    gui_lock_double_buffer();

    /* Setup log box */
    category435_setup_log_box(log_buffer, log_buffer_size);

    /* Setup editor box */
    CAT435_CTX->buffer = buffer;
    CAT435_CTX->buffer_size = buffer_size;
    if (CAT435_CTX->show_popup)
    {
        /* If IME is active. We need to abort it */
        category435_reset_editor_box();
        category435_setup_editor_box(buffer, buffer_size, CAT435_CTX->input_type, NULL);
    }

    gui_unlock_double_buffer();

    RedrawCategoryFunction();
}


/*****************************************************************************
 * FUNCTION
 *  Category435ShowPopupInternal
 * DESCRIPTION
 *  
 * PARAMETERS
 *  update_screen       [IN]        
 *  history_buffer      [?]         
 *  force_execution     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
	extern void ClearInputEventHandler(U16 device);
static void Category435ShowPopupInternal(BOOL update_screen, U8 *history_buffer, BOOL force_execution)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (CAT435_CTX->show_popup && !force_execution)
    {
        /* Popup already displayed */
        return;
    }

    CAT435_CTX->show_popup = MMI_TRUE;

#ifdef __MMI_TOUCH_SCREEN__
    wgui_enable_horizontal_tab_bar_pen_events(MMI_FALSE);
#endif 

    gui_lock_double_buffer();

    ClearInputEventHandler(MMI_DEVICE_KEY);
    change_left_softkey(CAT435_CTX->editor_left_softkey, CAT435_CTX->editor_left_softkey_icon);
    change_right_softkey(CAT435_CTX->editor_right_softkey, CAT435_CTX->editor_right_softkey_icon);
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();

    category435_setup_editor_box(CAT435_CTX->buffer, CAT435_CTX->buffer_size, CAT435_CTX->input_type, history_buffer);

    gui_unlock_double_buffer();

    if (update_screen)
    {
        RedrawCategoryFunction();
    }
}


/*****************************************************************************
 * FUNCTION
 *  Category435ShowPopup
 * DESCRIPTION
 *  
 * PARAMETERS
 *  update_screen       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category435ShowPopup(BOOL update_screen)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Category435ShowPopupInternal(update_screen, NULL, MMI_FALSE);
}


/*****************************************************************************
 * FUNCTION
 *  Category435HidePopupInternal
 * DESCRIPTION
 *  
 * PARAMETERS
 *  update_screen       [IN]        
 *  force_execution     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
	extern void ClearInputEventHandler(U16 device);
static void Category435HidePopupInternal(BOOL update_screen, BOOL force_execution)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!CAT435_CTX->show_popup && !force_execution)
    {
        /* Popup already hidden */
        return;
    }

    CAT435_CTX->show_popup = MMI_FALSE;

#ifdef __MMI_TOUCH_SCREEN__
    wgui_enable_horizontal_tab_bar_pen_events(MMI_TRUE);
#endif 

    gui_lock_double_buffer();

    /* Reset editor (cf. RedrawCategory435Screen) */
    category435_reset_editor_box();
    ClearInputEventHandler(MMI_DEVICE_KEY);
    change_left_softkey(CAT435_CTX->left_softkey, CAT435_CTX->left_softkey_icon);
    change_right_softkey(CAT435_CTX->right_softkey, CAT435_CTX->right_softkey_icon);
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();

    SetKeyHandler(category435_log_box_previous_line, KEY_UP_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(category435_log_box_next_line, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(category435_log_box_previous_line, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(category435_log_box_next_line, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(CAT435_CTX->LSK_func, KEY_EVENT_UP);
    SetRightSoftkeyFunction(CAT435_CTX->RSK_func, KEY_EVENT_UP);

    gui_unlock_double_buffer();

    if (update_screen)
    {
        RedrawCategoryFunction();
    }
}


/*****************************************************************************
 * FUNCTION
 *  Category435HidePopup
 * DESCRIPTION
 *  
 * PARAMETERS
 *  update_screen       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category435HidePopup(BOOL update_screen)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Category435HidePopupInternal(update_screen, MMI_FALSE);
}

/* Check if popup editor in category 435 is displayed */


/*****************************************************************************
 * FUNCTION
 *  Category435IsPopupDisplayed
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
BOOL Category435IsPopupDisplayed(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* ShowCategory435Screen should be invoked first */
    MMI_ASSERT(CAT435_CTX);

    return CAT435_CTX->show_popup;
}

/* Typically used to insert mood symbols. Not efficient for inserting long string */


/*****************************************************************************
 * FUNCTION
 *  Category435DirectInsertString
 * DESCRIPTION
 *  
 * PARAMETERS
 *  s                   [IN]        
 *  redraw_screen       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category435DirectInsertString(UI_string_type s, BOOL redraw_screen)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, l;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* ShowCategory435Screen should be invoked first */
    MMI_ASSERT(CAT435_CTX);

    gui_lock_double_buffer();

    l = gui_strlen(s);
    for (i = 0; i < l; i++)
    {
        if (MMI_multiline_inputbox.text_length >= MMI_multiline_inputbox.available_length)
        {
            gui_multi_line_input_box_insert_character(&MMI_multiline_inputbox, s[i]);
            break;
        }
        else
        {
            gui_multi_line_input_box_insert_character(&MMI_multiline_inputbox, s[i]);
        }
    }

    multiline_inputbox_input_callback();

    category435_sync_with_T9(); /* While double buffer locked */

    gui_unlock_double_buffer();

    if (redraw_screen && CAT435_CTX->show_popup)
    {
        wgui_show_inputbox();
    }
}


/*****************************************************************************
 * FUNCTION
 *  Category435BlinkTab
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tab     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category435BlinkTab(S8 tab)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_horizontal_tab_bar_start_blinking(tab);
}


/*****************************************************************************
 * FUNCTION
 *  Category435UnBlinkTab
 * DESCRIPTION
 *  
 * PARAMETERS
 *  tab     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Category435UnBlinkTab(S8 tab)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_horizontal_tab_bar_stop_blinking(tab);
}


/*****************************************************************************
 * FUNCTION
 *  UpdateCategory435TabBar
 * DESCRIPTION
 *  
 * PARAMETERS
 *  title               [IN]        
 *  n_tabs              [IN]        
 *  highlighted_tab     [IN]        
 *  tab_items           [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void UpdateCategory435TabBar(UI_string_type title, S8 n_tabs, S8 highlighted_tab, tab_bar_item_type *tab_items)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!CAT435_CTX)
    {
        MMI_DBG_ASSERT(0);
        return;
    }

    if (title == NULL)
    {
        title = MMI_horizontal_tab_bar.hint;
    }

    gui_lock_double_buffer();

    wgui_close_horizontal_tab_bar();

    /* Setup horizontal tab bar */
    for (i = 0; i < n_tabs; i++)
    {
        MMI_tab_bar_items[i] = tab_items[i];
    }
    wgui_create_horizontal_tab_bar(TRUE, title, n_tabs, highlighted_tab, MMI_TRUE);

    gui_unlock_double_buffer();

#ifdef CAT435_POPUP_OVERLAP_TAB_BAR
    if (CAT435_CTX->show_popup)
    {
        wgui_show_horizontal_tab_bar_tab_area();
    }
    else
    {
        wgui_show_horizontal_tab_bar();
    }
#else /* CAT435_POPUP_OVERLAP_TAB_BAR */ 
    wgui_show_horizontal_tab_bar();
#endif /* CAT435_POPUP_OVERLAP_TAB_BAR */ 
}


/*****************************************************************************
 * FUNCTION
 *  SetCategory435TabSelectCallback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  select_callback     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void SetCategory435TabSelectCallback(void (*select_callback) (int index))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_DBG_ASSERT(g_mmi_gui_cat435_context_p);
    wgui_set_horizontal_tab_bar_select_callback(select_callback);
}


/*****************************************************************************
 * FUNCTION
 *  ShowCategory435Screen
 * DESCRIPTION
 *  Displays the category435 screen
 *  
 *  1. Category 435 is composed by one background log and one popup text editor.
 *  2. icon_callback is typically set as wgui_general_multiline_icon_handler()
 *  (configured by wgui_config_general_multiline_icon_handler)
 * PARAMETERS
 *  title                           [IN]        Title for the screen
 *  left_softkey                    [IN]        Left softkey label
 *  left_softkey_icon               [IN]        Icon for the left softkey
 *  right_softkey                   [IN]        Right softkey label
 *  right_softkey_icon              [IN]        Icon for the right softkey
 *  editor_left_softkey             [IN]        Left softkey label when popup editor is active
 *  editor_left_softkey_icon        [IN]        Icon for the left softkey when popup editor is active
 *  editor_right_softkey            [IN]        Right softkey label when popup editor is active
 *  editor_right_softkey_icon       [IN]        Icon for the right softkey when popup editor is active
 *  n_tabs                          [IN]        Number of tabs
 *  highlighted_tab                 [IN]        Highlighted tab
 *  tab_items                       [IN]        Tab list
 *  show_popup                      [IN]        Whether to display popup editor
 *  popup_title                     [IN]        The title of popup editor
 *  input_type                      [IN]        Input method type
 *  buffer                          [IN]        Buffer the text input
 *  buffer_size                     [IN]        Character count of the text input buffer. (without '\0')
 *  log_buffer                      [IN]        Buffer the background log
 *  log_buffer_size                 [IN]        Character count of the background log buffer.  (without '\0')
 *  icon_callback                   [IN]        Callback to translate a string to an icon
 *  sample_icon                     [IN]        
 *  history_buffer                  [IN]        History buffer
 *  NULL(?)                         [IN]        If no icon is required.
 *  For(?)                          [IN]        Example, 5 means only 5 characters (5*2 + 2 bytes) can be input in this screen.
 * RETURNS
 *  void
 *****************************************************************************/
void ShowCategory435Screen(
        UI_string_type title,
        U16 left_softkey,
        U16 left_softkey_icon,
        U16 right_softkey,
        U16 right_softkey_icon,
        U16 editor_left_softkey,
        U16 editor_left_softkey_icon,
        U16 editor_right_softkey,
        U16 editor_right_softkey_icon,
        S8 n_tabs,
        S8 highlighted_tab,
        tab_bar_item_type *tab_items,
        BOOL show_popup,
        UI_string_type popup_title,
        S16 input_type,
        U8 *buffer,
        S32 buffer_size,
        U8 *log_buffer,
        S32 log_buffer_size,
        multi_line_input_box_icon_hdlr icon_callback,
        PU8 sample_icon,
        U8 *history_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int i;
    S32 sample_icon_width = 0, sample_icon_height = 0;
    dm_data_struct dm_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_gui_cat435_context_p = OslMalloc(sizeof(gui_cat435_context_struct));
    CAT435_CTX->right_softkey = right_softkey;
    CAT435_CTX->right_softkey_icon = right_softkey_icon;
    CAT435_CTX->left_softkey = left_softkey;
    CAT435_CTX->left_softkey_icon = left_softkey_icon;
    CAT435_CTX->editor_right_softkey = editor_right_softkey;
    CAT435_CTX->editor_right_softkey_icon = editor_right_softkey_icon;
    CAT435_CTX->editor_left_softkey = editor_left_softkey;
    CAT435_CTX->editor_left_softkey_icon = editor_left_softkey_icon;
    CAT435_CTX->popup_title = popup_title;
    CAT435_CTX->editor_RSK_func = UI_dummy_function;
    CAT435_CTX->editor_LSK_func = UI_dummy_function;
    CAT435_CTX->LSK_func = UI_dummy_function;
    CAT435_CTX->RSK_func = UI_dummy_function;
    /* We might need to setup/close editor later */
    CAT435_CTX->buffer = buffer;
    CAT435_CTX->buffer_size = buffer_size;
    CAT435_CTX->editor_created = 0;
    CAT435_CTX->input_type = input_type;
    CAT435_CTX->icon_callback = icon_callback;
    gui_measure_image(sample_icon, &sample_icon_width, &sample_icon_height);
    CAT435_CTX->icon_height = (S8) sample_icon_height;

    gui_lock_double_buffer();

    MMI_menu_shortcut_number = -1;
    MMI_disable_title_shortcut_display = 1;
    MMI_title_string = NULL;
    MMI_title_icon = NULL;

    clear_category_screen_key_handlers();
    clear_left_softkey();
    clear_right_softkey();
    register_left_softkey_handler();
    register_right_softkey_handler();
    register_default_hide_softkeys();

    /* Setup horizontal tab bar */
    for (i = 0; i < n_tabs; i++)
    {
        MMI_tab_bar_items[i] = tab_items[i];
    }
    wgui_create_horizontal_tab_bar(TRUE, title, n_tabs, highlighted_tab, MMI_FALSE);

    /* Setup log area */
    category435_setup_log_box(log_buffer, log_buffer_size);

    /* Setup editor area */
    CAT435_CTX->backup_MMI_editor_width = MMI_multiline_inputbox_width;
    CAT435_CTX->backup_MMI_editor_height = MMI_multiline_inputbox_height;
    CAT435_CTX->backup_information_bar_color = current_information_bar_theme->background_color;
    CAT435_CTX->backup_multitap_filler = current_multitap_input_theme->f;

    current_information_bar_theme->background_color = g_mmi_gui_cat435_popup_color;
    current_multitap_input_theme->f = (UI_filled_area*)&g_mmi_gui_cat435_multitap_filler;
    set_MMI_multitap_theme();

    if (history_buffer)
    {
        /* Check the first four bytes on whether popup exists */
        gui_cat435_history_header_struct *h = (gui_cat435_history_header_struct*) history_buffer;

        show_popup = (BOOL) (h->show_popup ? MMI_TRUE : MMI_FALSE);
        if (show_popup)
        {
            history_buffer += sizeof(gui_cat435_history_header_struct);
        }
        else
        {
            /* No history for editor */
            history_buffer = NULL;
        }
    }

    /* Setup key */
    if (show_popup)
    {
        Category435ShowPopupInternal(MMI_FALSE, history_buffer, MMI_TRUE);
    }
    else
    {
        Category435HidePopupInternal(MMI_FALSE, MMI_TRUE);
    }

#ifdef __MMI_WALLPAPER_ON_BOTTOM__
    dm_get_scr_bg_image(
        &editor_scr_bg_ID,
        &editor_scr_bg_filename,
        &editor_scr_bg_x,
        &editor_scr_bg_y,
        &editor_scr_bg_opacity);
    dm_set_scr_bg_image(
        editor_scr_bg_ID,
        editor_scr_bg_filename,
        editor_scr_bg_x,
        editor_scr_bg_y,
        MMI_EDITOR_BG_OPACITY);
#endif /* __MMI_WALLPAPER_ON_BOTTOM__ */ 

    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategory435Screen;
    dm_register_category_controlled_callback(category435_draw_controlled_area);
    /* Do not use dm_get_category_history{_size} */
    dm_setup_category_functions(dm_redraw_category_screen, GetCategory435History, GetCategory435HistorySize);
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY435_ID;
    dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
    dm_data.S32flags |= DM_SHOW_VKPAD;
#ifdef __MMI_TOUCH_SCREEN__
    wgui_register_category_screen_control_area_pen_handlers(
        category435_control_area_pen_down_handler,
        MMI_PEN_EVENT_DOWN);
    wgui_register_category_screen_control_area_pen_handlers(category435_control_area_pen_up_handler, MMI_PEN_EVENT_UP);
    wgui_register_category_screen_control_area_pen_handlers(
        category435_control_area_pen_move_handler,
        MMI_PEN_EVENT_MOVE);
    wgui_register_category_screen_control_area_pen_handlers(
        category435_control_area_pen_repeat_handler,
        MMI_PEN_EVENT_REPEAT);
    wgui_register_category_screen_control_area_pen_handlers(
        category435_control_area_pen_longtap_handler,
        MMI_PEN_EVENT_LONG_TAP);
    wgui_register_category_screen_control_area_pen_handlers(
        category435_control_area_pen_abort_handler,
        MMI_PEN_EVENT_ABORT);
#endif /* __MMI_TOUCH_SCREEN__ */ 
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();

}

#endif
